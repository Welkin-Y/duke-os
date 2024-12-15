
# Running Linux on Windows or MacOS Computers

We recommend the use of Docker Desktop, if your system:

* is reasonably new (maybe 4 years),
* is reasonably up to date,
* has adequate free disk space (ten gigabytes should do it). 

Docker lets you run a Linux development environment of your choice. Under the hood, Docker Desktop launches a virtual machine (guest) on your computer that runs alongside the host operating system installed on your computer. The guest VM runs a Linux OS. Docker also installs application software. You can use that software to build a Docker image and launch a Linux container in the guest VM from the image.

Matching your dev container to the AG test environment. In particular, docker permits you to run a development environment that matches the AG configuration used for CPS 310, by using the docker configuration file (Dockerfile) that we provide. Matching is helpful because programs that work in one configuration (e.g., yours) may fail in others (e.g., AG), and the root causes may be concealed from you. In particular, the AG runs Ubuntu 18 and is restricted to classic C++. Newer Ubuntu systems support newer C++ features that are not compatible with classic C++. Programs that use these features fail under the AG. The AG does not tell you why a program failed; it just says "did not run".

The docker command. Once you have installed Docker Desktop, the best way to manage your containers is to run the docker command in a terminal window on your host computer. There is extensive documentation on the docker command. It is a "swiss army knife" with many arguments and multiple ways to do the same thing. We provide specific instructions below.

# Understanding Docker

Docker can be confusing. Apart from all the virtual magic, the basic mental model is hard to get straight from all the words about it on the web. It is important to be precise with terminology.

The key thing to understand is that a container has a bag of files (a file tree) and zero or more active processes. The file tree is a customized (layered) view of a Linux file system. The container's file tree is seeded with a logical copy of a file tree in the container's initial image. But the file tree also includes any files created by processes running within the container. Any process that runs within the container sees the container's file tree, and may modify it. You can run many containers from the same image, and they each have their own file tree. If a process modifies the file tree in one container, those changes are not visible from other containers.

It is easy to create a new container, even by accident. You can use docker run to run a command (process) within a container, say, bash or another command shell. You use the -it flags to run the container/process in interactive mode, so that you can type commands to it and see the output directly in your terminal window. But docker run always creates a new container, so any files created by a previous run are no longer visible to the new process.

If your process in the container exits (e.g., you exit your shell), then the container becomes dormant and you return to your host shell. But the container's bag of files remains, and you can use docker start to launch another process (e.g., another shell) within the same container, where the files are visible. If you want to reclaim the disk space, you can delete the container's files with the docker rm command. If you are finished and you want to delete an image, use the command docker image prune after deleting (with docker rm) any containers created from the image.

So: to understand docker, it is important to distinguish in your mind three things: the image, the container's process (or processes), and the container's bag of files (its file layers or file tree or file system). There is also a Dockerfile, which is an ordinary text file of commands to configure the image. That's four kinds of objects, each with its own lifecycle, and with dependency relationships among them.

# Sharing files with your container

Conveniently, a container's file tree may also include bind mounts, which attach folders/directories in the host file system so that processes in the container can operate on them. You select a directory that exists in your host file system and then command docker to bind mount it into a container's file tree, so that the directory is visible (usually at a different pathname) to processes running in the container. Then, if you destroy the container, then the mounted directory and its files survive in your host file tree. You can even share the directory among multiple containers over time or at the same time.

It is useful to bind mount your git local repository into your container, so that you can edit source files on the host using your favorite editor application, and then build/run them in the Linux container. You can run git commands on the host or in your container. But you have to do the git config and install your keys and so on in both places.
Docker setup for CPS310/510

[Here is a Dockerfile for CPS310/510 Fall 2020.](Dockerfile)

[There is also a companion video showing how to setup docker desktop and visual studio code for an easy-to-use environment on Windows.](https://compsci.capture.duke.edu/Panopto/Pages/Viewer.aspx?id=ce1bcf37-5b25-4b5a-85c1-aca80021d12b)

These sample commands choose various names with 310 in them. You can use whatever names you want.

Step 1. Build a container image on your host. Create an empty directory/folder and download/copy the Dockerfile into it with the name Dockerfile. Then in a terminal window on your host, enter the directory (with cd) and type:

    docker build -t dev310 .

Don't forget the . (current directory)! That build command creates an image based on the Dockerfile, and gives the image the name (tag) dev310. It downloads packages from the network and generates a lot of output. You might see some red messages "debconf: delaying package configuration, since apt-utils is not installed", but that is just a quirk of Debian/Ubuntu and can be ignored. (Any other red messages may indicate a problem.) If everything goes well you have a named image. You don't see it, but it is there.

Step 2. Create a directory/folder on your host to bind mount into your container(s). Use the mkdir command or select new folder in the menu of a window on the containing folder. In a host terminal window, change into that directory (cd) and clone up a local git repository in the usual fashion with git clone ....

Step 3. Create a container from the image and attach the bind mount. Suppose the new directory/folder is called /Users/chase/docker-share (use pwd to find out), and you want the bind mount to appear in the container file tree at /cps310:

    docker run --name cps310 --mount type=bind,source=/Users/chase/docker-share,destination=/cps310 -it dev310 bash

That creates a new container from the dev310 image, and names the container cps310. The new container is interactive (-i) and it has a terminal interface (-t), and it runs a bash shell process. The next command prompt you see should be the linux command shell (bash) in your new container. You can type Linux commands to it all day long. Change directory (cd) to your /cps310 bind mount, and go to town.

NOTE: newer versions of Docker Desktop for windows require explicit permission before mounting is possible. See [this link for how to make your directory mountable](https://stackoverflow.com/questions/60754297/docker-compose-failed-to-build-filesharing-has-been-cancelled).


# Maintaining your container

Installing software. While running in your container, you may find that various tools are missing. You can use the apt-get command to install them.

Getting another shell in the same container. Instead of using docker run (which starts a new container), use docker exec. E.g.: 

    docker exec -ti cps310 bash

This one is really useful if you've got a test program running in one container, but you need to do something in another (e.g. kill it with the command).

## Restarting an existing container

If you exit the shell, reboot your computer, or destroy the terminal window, don't worry. Everything is OK. Your container is still there. More precisely, your processes (e.g., your command shell) are dead, but the container's file tree is still there. You can get back into it by saying:

    docker start -i cps310

That starts your existing named container again, using the same configuration and command as specified in docker run that created the container. Specifically, it launches a new bash shell into the existing cps310 container in interactive mode (-i). Again, you get a bash command prompt for a shell running in the container, and everything should look just as it did.

Remember if you use docker run again it creates a new container, or it gives you an error for running two containers with the same name. A new container might be OK if the only files you modified are in the bind mount directory (/cps310), but it takes up more space, and any files you modified in the old container (e.g., to install missing packages with apt-get) do not appear in the new container.

Other things you can do:

    docker ps -a: List all of your current containers.
    docker rm NAME: Destroy the container with NAME and its file system, killing any processes in the container. Any directories that were bind mounts in the container are preserved on your host. You might have to stop it first, e.g.: docker stop NAME
    docker image prune: Destroy any images that are not currently in use for any container. 

For much more, see the Docker documentation or google around on the Web. 

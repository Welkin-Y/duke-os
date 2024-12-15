<?php

##$DATE_FORMAT = "F d Y H:i:s";         # August 18 2009 13:46:24
##$DATE_FORMAT = "M d, Y h:i a";        # August 18 2009 01:46 pm
$DATE_FORMAT = "M d, Y H:i";            # Aug 18, 2009 13:46

# Add files to exlude to EXCLUDE array, this script is
# automatically included
$EXCLUDE = array('.','..','.subdir','SCCS','.htaccess','.DS_Store','index.php.save','protected','.AppleDouble','.index.php.swp');


$dir = shell_exec("pwd");
$dir = basename($dir);

?>

<!--<div id="Content600Wide">-->
<div id="ContentWrapper" style="margin-top: 30px;">

<div id="LeftColumn">
</div>
<div id="MainColumn" style="padding-right:200px;">

<h1 id="PageTitle"><?= $dir ?></h1>

<?php

if (is_file('.subdir')) {
        print "<h3>$dir</h3>";
}
?>

<table style="width: 100%; width: 400px; border-collapse: collapse; empty-cells: show;">
<tr>
<th style='padding-right: 10px'>&nbsp;</th>
<th style='padding-right: 10px; text-decoration: underline; width: 50%;'>File</th>
<th style='text-decoration: underline;'>Modified</th>
</tr>

<?php

# if this is a subdirectory, allow us to navigate up
if (is_file('.subdir')) {
        $COLOR = "dark";
        echo "<tr class='${COLOR}'>";
        echo "<td style='padding-right: 10px'>d</td>";
        echo "<td style='padding-right: 10px'>";
        echo "<a href='..'>Parent Directory</a></td>";
        echo "<td nowrap>" . date ($DATE_FORMAT, filemtime('..')) . "</td>";
        echo "</tr>\n";
}

# Add script to EXCLUDE list
array_push($EXCLUDE, basename($_SERVER['SCRIPT_FILENAME']));

# Unset the EXCLUDE list, if requested
if ($_REQUEST['showall'])
        $EXCLUDE = array();

# Open directory and read contents into an array
$CONTENTS = array();
if ($handle = opendir('.')) {
    while (false !== ($file = readdir($handle))) {
        array_push($CONTENTS, $file);
    }
    closedir($handle);
}

# if array not empty, now write it out (this allows for sorting)
if (count($CONTENTS) > 0 ) {

    asort($CONTENTS);

    foreach ($CONTENTS as $file) {
        if (! in_array($file,$EXCLUDE) and !(preg_match("/\._.+/i", $file))) {
                $COLOR = ( $COLOR == "dark" ) ? "light" : "dark";

                $url = rawurlencode($file);
                $stat = stat($file);

                echo "<tr class='${COLOR}'>";
                if (is_dir($file))
                        echo "<td style='padding-right: 10px;'>d</td>";
                else
                        echo "<td style='padding-right: 10px'>&#164;</td>";

                echo "<td style='padding-right: 10px;'>";
                echo "<a href='" . $url . "'>" . $file . "</a>";
                is_dir($file) and print "/";
                echo "</td>";
                echo "<td nowrap>" . date ($DATE_FORMAT, filemtime($file)) . "</td>";

                echo "</tr>\n";
        }
    }
}

print "</table>\n";
print "</div>\n";
print "</div>\n";

?>
       

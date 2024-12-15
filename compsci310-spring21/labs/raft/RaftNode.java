
public class RaftNode {

	private RaftLog mLog;
	private Timer mTimer;
	private AppendEntryRequester mAppendEntryRequester;
	private VoteRequester mVoteRequester;
	private int mNumServers;
	private int mNodeServerNumber;
	private RaftMode mCurrentMode;
	private int mTerm;
	private int mCommitIndex;


	/**
	 * RaftNode constructor, called by test harness.
	 * @param log this replica's local log, pre-populated with entries
	 * @param timer this replica's local timer
	 * @param sendAppendEntry call this to send RPCs as leader and check responses
	 * @param sendVoteRequest call this to send RPCs as candidate and check responses
	 * @param numServers how many servers in the configuration (numbered starting at 0)
	 * @param nodeServerNumber server number for this replica (this RaftNode object)
	 * @param currentMode initial mode for this replica
	 * @param term initial term (e.g., last term seen before failure).  Terms start at 1.
	 **/
	public RaftNode(
			RaftLog log,
			Timer timer,
			AppendEntryRequester sendAppendEntry,
			VoteRequester sendVoteRequest,
			int numServers,
			int nodeServerNumber,
			RaftMode currentMode,
			int term) {
		mLog = log;
		mTimer = timer;
		mAppendEntryRequester = sendAppendEntry;
		mVoteRequester = sendVoteRequest;
		mNumServers = numServers;
		mNodeServerNumber = nodeServerNumber;
		mCurrentMode = currentMode;
		mTerm = term;
		mCommitIndex = -1;

	}

	public RaftMode getCurrentMode() {
		return mCurrentMode;
	}

	public int getCommitIndex() {
		return mCommitIndex;
	}

	public int getTerm() {
		return mTerm;
	}

	public RaftLog getCurrentLog() {
		return mLog;
	}

	public int getServerId() {
		return mNodeServerNumber;
	}

	public int getLastApplied() {
		return mLog.getLastEntryIndex();
	}

	public boolean votedInTerm(int term) {
	// Return whether or not this RaftNode has voted in specified term.
	// Note: called only for current term or future term.
	   	 return false;
	}

	/**
	 * @param candidateTerm candidate's term
	 * @param candidateID   candidate requesting vote
	 * @param lastLogIndex  index of candidate's last log entry
	 * @param lastLogTerm   term of candidate's last log entry
	 * @return 0, if server votes for candidate; otherwise, server's current term
	 */
	public int receiveVoteRequest(int candidateTerm,
								  int candidateID,
								  int lastLogIndex,
								  int lastLogTerm) {
		return -1;
	}

	/**
	 * @param leaderTerm   leader's term
	 * @param leaderID     current leader
	 * @param prevLogIndex index of log entry before entries to append
	 * @param prevLogTerm  term of log entry before entries to append
	 * @param entries      entries to append (in order of 0 to append.length-1)
	 * @param leaderCommit index of highest committed entry
	 * @return 0, if follower accepts; otherwise, follower's current term
	 */
	public int receiveAppendEntry(int leaderTerm,
								  int leaderID,
								  int prevLogIndex,
								  int prevLogTerm,
								  Entry[] entries,
								  int leaderCommit) {
		return -1;
	}

	public int handleTimeout() {
		return -1;
	}

	/**
	 *  Private method shows how to issue a round of RPC calls.
	 *  Responses come in over time: after at least one timer interval, call mVoteRequester to query/retrieve responses.
	 */
	private void requestVotes() {
		for (int i = 0; i < mNumServers; i++) {
			if (i != mNodeServerNumber) {
				mVoteRequester.send(i, mTerm, mNodeServerNumber, mLog.getLastEntryIndex(), mLog.getLastEntryTerm());
			}
		}
	}


}


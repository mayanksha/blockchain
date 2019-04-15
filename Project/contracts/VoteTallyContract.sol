pragma solidity ^0.5.1;

contract VoteTally {
  address public owner;

  constructor() public {
    owner = msg.sender;
  }

  mapping (address => uint) votes;

  function addVote(address addressOfCandidate) private {
  	require (msg.sender == owner)
  		votes[addressOfCandidate] = votes[addressOfCandidate] + 1;
  }

  function getVotesOfCandidate(address addressOfCandidate) private returns (uint) {
  	require (msg.sender == owner)
  		return votes[addressOfCandidate];
  }

}

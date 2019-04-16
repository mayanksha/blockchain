pragma solidity ^0.5.0;

contract VoteTally {
    address public owner;
    address public ECAddressCreator;

    constructor(address _ECHead) public {
        owner = msg.sender;
        ECAddressCreator = _ECHead;
    }

    mapping (address => uint) private votes;

    function castVote(address addressOfCandidate) public ManagerOnly returns (bool) {
        if (votes[addressOfCandidate] < 0) {
            return false;
        } else {
            votes[addressOfCandidate] = votes[addressOfCandidate] + 1;
            return true;
        }
    }

    function getVotesOfCandidate(address addressOfCandidate) public view ManagerOnly returns (uint) {
        return votes[addressOfCandidate];
    }

    function addCandidate(address addressOfCandidate) public ManagerOnly {
        require(votes[addressOfCandidate] == 0);
    }

    function getSelfContract() public view ManagerOnly returns (VoteTally) {
        return VoteTally(address(this));
    }

    function getSelfAddress() public view returns (address) {
        require (msg.sender == owner || msg.sender == ECAddressCreator);
        return address(this);
    }

    modifier ManagerOnly {
        require (msg.sender == owner || msg.sender == ECAddressCreator);
        _; 
    }
}

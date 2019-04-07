pragma solidity ^0.5.1;

contract Voter {
  address public owner;
  //mapping (address => )

  // TODO: 1. set head of EC 
  constructor() public {
    owner = msg.sender;
  }

  address private createdByECWorker;
  string public Name;
  string private PermanentAddress;

  // Stored as a timestamp
  uint private DOB;
  string private FathersName;
  string private MothersName;

  // Maybe we can swap it later for Hall of Residence for our case?
  string public ElectoralConstituency;

  string public RollNumber;
 
  // Dunno how we're gonna store the hash of the fingerprint
  // This is allowed to be modified only is the sender.msg is Manager 
  address private BiometricData;

  
}

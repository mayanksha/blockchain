pragma solidity ^0.5.1;

contract Voter {
    address private createdByECWorker;
    // Person whose Voter Contrac is being created
    address public owner;
    string public Name;
    string private PermanentAddress;

    // Stored as a timestamp
    uint private DOB;
    string private FathersName;
    string private MothersName;

    // Maybe we can swap it later for Hall of Residence for our case?
    //string public ElectoralConstituency;
    string public HallOfResidence;
    string public RollNumber;

    // Dunno how we're gonna store the hash of the fingerprint
    // This is allowed to be modified only is the sender.msg is Manager 
    address private BiometricData;

    constructor(
        address _personAddr,
        string _Name,
        string _FathersName,
        string _MothersName,
        string _HallOfResidence,
        string _RollNumber,
        uint _DOB
    ) public {
        createdByECWorker = msg.sender;
        owner =  _personAddrs;
        Name = _Name;
        FathersName = _FathersName;
        MothersName = _MothersName;
        HallOfResidence = _HallOfResidence;
        RollNumber = _RollNumber;
        DOB = _DOB;
    }

    function viewMyDetails() public returns (address, string, string, string, string, string, uint) {
        require (msg.sender == owner)
            return (owner, FathersName, MothersName, HallOfResidence, RollNumber, DOB);
    }

}

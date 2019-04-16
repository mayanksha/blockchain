pragma solidity ^0.5.0;

contract Voter {
    address private ECAddressCreator;
    // Person whose Voter Contrac is being created
    address public owner;
    string public Name;
    string private PermanentAddress;

    // Stored as a timestamp
    int private DOB;
    string private FathersName;
    string private MothersName;

    // Maybe we can swap it later for Hall of Residence for our case?
    //string public ElectoralConstituency;
    string public HallOfResidence;
    uint256 RollNumber;

    // Dunno how we're gonna store the hash of the fingerprint
    // This is allowed to be modified only is the sender.msg is Manager 
    address private BiometricData;

    constructor(
        address _personAddr,
        string memory _Name,
        string memory _FathersName,
        string memory _MothersName,
        string memory _HallOfResidence,
        uint256 _RollNumber,
        int _DOB
    ) public {
        ECAddressCreator = msg.sender;
        owner =  _personAddr;
        Name = _Name;
        FathersName = _FathersName;
        MothersName = _MothersName;
        HallOfResidence = _HallOfResidence;
        RollNumber = _RollNumber;
        DOB = _DOB;
    }

    function viewMyDetails() public returns (address, string memory, string memory, string memory, string memory, uint256, int) {
        require (msg.sender == owner);
        return (owner, Name, FathersName, MothersName, HallOfResidence, RollNumber, DOB);
    }

    function getSelfContract() public returns (Voter) {
        require (msg.sender == owner || msg.sender == ECAddressCreator);
        return Voter(address(this));
    }

    function getSelfAddress() public returns (address) {
        require (msg.sender == owner || msg.sender == ECAddressCreator);
        return address(this);
    }
}

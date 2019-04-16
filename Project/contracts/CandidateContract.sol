pragma solidity ^0.5.0;

contract Candidate {
    address public owner;
    address ECAddressCreator;

    // Later conert then to arrays
    address[] proposers;
    address[] seconders;

    string manifesto;
    string credentials;
    string[] penalties;

    string Name;
    int RollNo;

    struct penalty {
        uint32 fineAmount;

    }
    constructor(
        address personAddr,
        string memory _Name,
        int _r,
        address[] memory prop,
        address[] memory sec,
        string memory mani,
        string memory cred
    ) public {
        ECAddressCreator = msg.sender;
        owner = personAddr;
        proposers = prop;
        seconders = sec;
        manifesto = mani;
        cred = credentials;
        Name = _Name;
        RollNo = _r;
    }

    function getManifesto() public view returns (string memory) {
        return manifesto;
    }
    function getName() public view returns (string memory) {
        return Name;
    }
    function getRollNo() public view returns (int) {
        return RollNo;
    }
    //function getSeconders() public view returns (string memory) {
        //string retVal = "";
        //for (uint i = 0; i < seconders.length; i++) {
            ////strConcat("", )
        //};
        //return seconders;
    //}
    //function getProposers() public view returns (string memory) {
        //return proposers;
    //}


    function strConcat(string memory _a, string memory _b) internal returns (string memory) {
        bytes memory _ba = bytes(_a);
        bytes memory _bb = bytes(_b);
        string memory abcde = new string(_ba.length + _bb.length );
        bytes memory babcde = bytes(abcde);
        uint k = 0;
        for (uint i = 0; i < _ba.length; i++) babcde[k++] = _ba[i];
        for (uint i = 0; i < _bb.length; i++) babcde[k++] = _bb[i];
        return string(babcde);
    }

    function getSelfContract() public returns (Candidate) {
        require (msg.sender == owner || msg.sender == ECAddressCreator);
        return Candidate(address(this));
    }
    
    function getSelfAddress() public returns (address) {
        require (msg.sender == owner || msg.sender == ECAddressCreator);
        return address(this);
    }
}

pragma solidity ^0.5.1;

contract Candidate {
    address public owner;
    address createdBy;

    // Later conert then to arrays
    string[] proposers;
    string[] seconders;

    string manifesto;
    string credentials;
    string[] penalties;

    struct penalty {
        uint32 fineAmount;

    }
    constructor(
        address personAddr,
        address[] prop,
        address[] sec,
        string mani,
        string cred
    ) public {
        createdBy = msg.sender;
        owner = personAddr;
        proposers = prop;
        seconders = sec;
        manifesto = mani;
        cred = credentials;
    }

    function getManifesto() public view returns (string memory) {
        return manifesto;
    }
    function getSeconders() public view returns (string memory) {
        string retVal = "";
        for (uint i = 0; i < seconders.length; i++) {
            //strConcat("", )
        };
        return seconders;
    }
    function getProposers() public view returns (string memory) {
        return proposers;
    }


    function strConcat(string _a, string _b) internal returns (string){
        bytes memory _ba = bytes(_a);
        bytes memory _bb = bytes(_b);
        string memory abcde = new string(_ba.length + _bb.length );
        bytes memory babcde = bytes(abcde);
        uint k = 0;
        for (uint i = 0; i < _ba.length; i++) babcde[k++] = _ba[i];
        for (i = 0; i < _bb.length; i++) babcde[k++] = _bb[i];
        return string(babcde);
    }

}

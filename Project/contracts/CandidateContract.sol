pragma solidity ^0.5.0;

contract Candidate {
    address public owner;
    address createdBy;

    // Later conert then to arrays
    address[] proposers;
    address[] seconders;

    string manifesto;
    string credentials;
    string[] penalties;

    struct penalty {
        uint32 fineAmount;

    }
    constructor(
        address personAddr,
        address[] memory prop,
        address[] memory sec,
        string memory mani,
        string memory cred
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

}

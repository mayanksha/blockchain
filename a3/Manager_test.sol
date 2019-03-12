pragma solidity 0.5.1;
import "Manager.sol"; // this import is automatically injected by Remix.

// file name has to end with '_test.sol'
contract test_1 {
	address addr_1 = 0xca35b7d915458ef540ade6068dfe2f44e8fa733c;
	address addr_2 = 0x14723a09acff6d2a60dcdf7aa4aff308fddc160c;
	address addr_3 = 0x4b0897b0513fdc7c541b6d9d7e929c4e5364d2db;
	address addr_4 = 0x583031d1113ad414f02576bd6afabfb302140225;
	address addr_5 = 0xdd870fa1b7c4700f2bd7f44238821c26f7392148;
	address addr_6 = 0xab2dbe9ea47e34e5b0951af22c8e2233ce597c9b;
	address addr_7 = 0xffab557b72e8bcc34be6153f31e0a27d39abdc7f;
	address addr_8 = 0xdd969cf1e91bac27ac26a151e6ef1d3e65642789;
	address addr_9 = 0xcec8627ca66318fe143cce767e826596725c0405;
	address addr_10 = 0x336f8ba6a93af6872f7a6af3267c1fecaa104bbd;
	address addr_11 = 0xee01b0d2cde0c7eafdf45613fc3548fbd5e39b9d;

	sender.msg = addr_1;
  function beforeAll() public {
    // here should instantiate tested contract
    Assert.equal(uint(4), uint(3), "error in before all function");
  }

  function check1() public {
    // use 'Assert' to test the contract
    Assert.equal(uint(2), uint(1), "error message");
    Assert.equal(uint(2), uint(2), "error message");
  }

  function check2() public view returns (bool) {
    // use the return value (true or false) to test the contract
    return true;
  }
}

contract test_2 {
 
  function beforeAll() public {
    // here should instantiate tested contract
    Assert.equal(uint(4), uint(3), "error in before all function");
  }

  function check1() public {
    // use 'Assert' to test the contract
    Assert.equal(uint(2), uint(1), "error message");
    Assert.equal(uint(2), uint(2), "error message");
  }

  function check2() public view returns (bool) {
    // use the return value (true or false) to test the contract
    return true;
  }
}

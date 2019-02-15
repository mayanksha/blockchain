pragma solidity ^0.5.0;

// Create a new Contract
contract MyFirstContract {
	// Declare balance as a uint (256 bit unsigned int) 
	uint balance;

	// View function getBalance - View functions promise to not make any changes to state of contract 
	// Return Value Type (uint) 
	function getBalance() public view returns (uint) {
		return balance;
	}

	// constructor sets balance 
	constructor() public {
		balance = 100;
	}
}

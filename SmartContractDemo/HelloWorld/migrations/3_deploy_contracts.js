const MyFirstContract = artifacts.require("./MyFirstContract.sol");

// Export the deployer of  MyFirstContract 
module.exports = function(deployer) {
	// Deploy the MyFirstContract on Network
	deployer.deploy(MyFirstContract);
}

const Manager = artifacts.require("Manager");

module.exports = function(deployer, network, accounts) {
  deployer.deploy(Manager);
};

const MyFirstContract = artifacts.require("MyFirstContract") 

contract('MyFirstContract', () => {
	it("Should always return 100 when getBalance() is called.", async () => {
		const firstContractInstance = await MyFirstContract.deployed()
		const balance = await firstContractInstance.getBalance()
		assert.equal(balance, 100,
			"Oops! Default balance wasn't 100");
		
	})
})

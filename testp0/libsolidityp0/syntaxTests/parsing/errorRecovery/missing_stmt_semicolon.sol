pragma solidity >=0.0.0;

// This is a contract with a number of syntax errors involving missing
// semicolons.

// Tokens are deleted until the next subsequent semicolon.

contract MetaCoin1 {
	mapping (address => uint) balances;

	event Transfer(address indexed _from, address indexed _to, uint256 _value);

	constructor() public {
		balances[tx.origin] = 10000;
	}

	function sendCoin(address receiver, uint amount) public returns(bool sufficient) {
		uint i;
		// "Return" missing its semicolon.
		if (balances[msg.sender] < amount) return false

		balances[msg.sender] -= amount;

		// "Statement" missing its semicolon
		balances[receiver] += amount
		emit Transfer(msg.sender, receiver, amount);

		//
		while (amount < 10) {
			// "Break" missing its semicolon
			if (amount > 1) break
			amount += 1;
			// "Continue" missing its semicolon
			continue
			if (amount > 2) break;
		}
		return true;
	}

	function getBalanceInEth(address addr) public view returns(uint){
		return ConvertLib.convert(getBalance(addr),2);
	}

	function getBalance(address addr) public view returns(uint) {
		return balances[addr];
	}
}
// ----
// ParserError: (560-561): Expected ';' but got identifier. Skipping to next ';'.
// ParserError: (678-679): Expected ';' but got 'emit'. Skipping to next ';'.
// ParserError: (785-786): Expected ';' but got identifier. Skipping to next ';'.
// ParserError: (862-863): Expected ';' but got 'if'. Skipping to next ';'.

pragma solidity 0.0.0;

contract mismatched_pragma {
	function sendCoin(address receiver, uint amount) public returns(bool sufficient) {
		amount +=
		return true;
	}
}
// ----
// ParserError: (151-157): Expected primary expression.
// Warning: (162-163): Recovered in Statement at ';'.

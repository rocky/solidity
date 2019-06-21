// An exmple with multiple errors.
// Most are caught by inserting an expected token.
// However some us S C Johnson recovery to
// skip over tokens.

pragma solidity >=0.0.0;

contract Error4 {
  constructor() public {
    balances[tx.origin] = 1 2; // missing operator
  }

  function sendCoin(address receiver, uint amount) public returns(bool sufficient) {
    if (balances[msg.sender] < amount) return false;
    balances[msg.sender] -= amount   // Missing ";"
    balances[receiver] += amount   // Another missing ";"
    emit Transfer(msg.sender  // truncated line
    return true;
  }


}
// ----
// ParserError: (248-249): Expected ';' but got 'Number'
// ParserError: (470-478): Expected ';' but got identifier
// ParserError: (528-532): Expected ';' but got 'emit'
// ParserError: (576-582): Expected ',' but got 'return'
// ParserError: (576-582): Expected primary expression.
// Warning: (587-588): Recovered in Statement at ';'.

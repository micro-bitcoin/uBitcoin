# if(signature)

## Description

Indicates if signature creation was successfull.

After constructing or obtaining a signature it makes sense to check whether it was successfull or not. Constructing signature from der encoded bytes or string can fail if der encoding was incorrect. When obtained from functions like `PrivateKey.sign()` or similar it will indicate if signing process was successfull.

## Syntax

`if (signature)`

## Parameters

Nothing

## Returns

`bool` : returns true if signature is ok. Return false if something went wrong.

## Example code

```cpp
#include <Bitcoin.h>

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ; // waiting for serial port
  }
  
  // valid der encoding:
  char hexSig[] = "3044022065017359a1e5d8035c3bd1bf84a237145cdb1f9f80c671fa3153a818805dab1f02205564b587296bac49aa1f47f108aa3c387e8fd90bdd7b93bc33c7ae05d042f7d6";
  // invalid der encoding:
  // char hexSig[] = "300f020592393857930206009564253453";

  // creating a signature from der encoding
  Signature sig(hexSig);

  if(sig){
    Serial.println("Signature parsed successfully");
  }else{
    Serial.println("Invalid signature encoding");
  }
}

void loop() {

}
```
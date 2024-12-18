/**
   @brief Build a shared secret (using Diffie-Hellman) between the private key on the device and an external public key.

   @param publicKeyHex: String. The public key of the other party.
                        The format must be 128 hex characters, 64 bytes (x,y). No `02`, `03` or `04` prefixes.
   @return CommandResponse
*/

String generateSharedSecret(String publicKeyHex) {
  // Reconstruct full public key if only X-coordinate is provided
  if (publicKeyHex.length() == 64) {
    publicKeyHex = reconstructPublicKey(publicKeyHex);
  }

  String privateKeyHex = global.privateKeys[global.activeKeyIndex];

  int byteSize =  32;
  byte privateKeyBytes[byteSize];
  fromHex(privateKeyHex, privateKeyBytes, byteSize);
  PrivateKey privateKey(privateKeyBytes);

  byte sharedSecret[32];

  byte publicKeyBin[64];
  fromHex(publicKeyHex, publicKeyBin, 64);
  PublicKey otherPublicKey(publicKeyBin, true);
  privateKey.ecdh(otherPublicKey, sharedSecret, false);

  return toHex(sharedSecret, sizeof(sharedSecret));
}

CommandResponse executeSharedSecret(String publicKeyHex) {
  String sharedSecretHex = generateSharedSecret(publicKeyHex);
  sendCommandOutput(COMMAND_SHARED_SECRET, sharedSecretHex);

  return {"Shared Secret", "sent..."};
}
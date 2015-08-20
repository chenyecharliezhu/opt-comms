/** Author : Charlie Chenye Zhu
 *           chenye@stanford.edu
 *
 *  Namespaces containing AddingChecksum and RemovingChecksum methods.
 *      Comsumed by opcommMessage class
 */
#include "opcommmessage.h"

#ifndef OPCOMMCHECKSUMMETHODS_H
#define OPCOMMCHECKSUMMETHODS_H

namespace opcommChecksumMethods {

    std::string addChecksum_NoChecksumMethod(const std::string &encryptedMessage);
    std::string rmChecksum_NoChecksumMethod(const std::string &checksumMessage);
    bool validateChecksum_NoChecksumMethod(const std::string &checksumMessage);

    static const unsigned int MINIMUM_LENGTH_LRC8 = 1;
    // Bitwise XOR
    std::string addChecksum_LRC8(const std::string &encryptedMessage);
    std::string rmChecksum_LRC8(const std::string &checksumMessage);
    bool validateChecksum_LRC8(const std::string &checksumMessage);

}

#endif // OPCOMMCHECKSUMMETHODS_H

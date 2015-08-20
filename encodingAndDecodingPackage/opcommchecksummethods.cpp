/** Author : Charlie Chenye Zhu
 *           chenye@stanford.edu
 *
 *  Namespaces containing AddingChecksum and RemovingChecksum methods.
 *      Comsumed by opcommMessage class
 */

#include "opcommchecksummethods.h"

namespace opcommChecksumMethods {

    std::string addChecksum_NoChecksumMethod(const std::string &encryptedMessage){
        return encryptedMessage;
    }

    std::string rmChecksum_NoChecksumMethod(const std::string &checksumMessage){
        return checksumMessage;
    }

    bool validateChecksum_NoChecksumMethod(const std::string &checksumMessage){
        return true;
    }

    std::string addChecksum_LRC8(const std::string &encryptedMessage){
        if (encryptedMessage.length() < MINIMUM_LENGTH_LRC8){
            return encryptedMessage;
        } else{
            std::vector<char> encMessage(encryptedMessage.begin(), encryptedMessage.end());
            char checkSequence = 0;
            for (unsigned int i = 0; i < encryptedMessage.length(); i++)
                checkSequence ^= encMessage[i];
            std::string checksumMessage = encryptedMessage + checkSequence;
            return checksumMessage;
        }
    }

    std::string rmChecksum_LRC8(const std::string &checksumMessage){
        if (checksumMessage.length() <= MINIMUM_LENGTH_LRC8){
            // No Checksum Message Added or Impossible Scenerio
            return checksumMessage;
        } else {
            std::string result(checksumMessage.begin(), checksumMessage.end() - 1);
            return result;
        }
    }

    bool validateChecksum_LRC8(const std::string &checksumMessage){
        if (checksumMessage.length() < MINIMUM_LENGTH_LRC8){
            // No Checksum Message Added
            return true;
        } else if (checksumMessage.length() == MINIMUM_LENGTH_LRC8){
            // Impossible Scenerio
            return false;
        } else{
            unsigned char ch = 0;
            for (unsigned int i = 0; i < checksumMessage.length(); i++)
                ch ^= checksumMessage[i];
            return ch == 0;
        }
    }

}

/** Author: Charlie Chenye Zhu
 *          chenye@stanford.edu
 *
 *  Base class for both opcommMessageReceived and opcommMessageSent
 *
 *  Defines and implements the class constructor, operator =, ==, !=
 *  and external IO interfaces
 *
 *  Internally handles the encryptions and checksum related functions
 */
#include "opcommmessage.h"
#include <iostream>
#include <fstream>
#include <limits.h>
#include <string>
#include <assert.h>

unsigned char opcommMessage::mask[] = {128, 64, 32, 16, 8, 4, 2, 1};

// Constructor
opcommMessage::opcommMessage(EncryptionMethods enMethod, ChecksumMethods ckMethod){
    this->encryption = enMethod;
    this->checksum = ckMethod;
    this->lengthMessageReadable = 0;
    this->lengthMessageEncrypted = 0;
    this->lengthMessageEncryptedChecksum = 0;
}

// Utility functions
opcommMessage::EncryptionMethods opcommMessage::getEncryptionMethod(){
    return this->encryption;
}

opcommMessage::ChecksumMethods opcommMessage::getChecksumMethod(){
    return this->checksum;
}

std::string opcommMessage::EncryptionMethodToString(){
    switch (encryption){
        case opcommMessage::EncryptionMethodNone:
            return "None";
            break;
        default:
            return "Please update EncryptionMethodToString method";
            break;
    }
}

std::string opcommMessage::ChecksumMethodToString(){
    switch (checksum){
        case opcommMessage::ChecksumMethodNone:
            return "None";
            break;
        default:
            return "Please update ChecksumMethodToString method";
            break;
    }
}

/**
 * IO Support
 *      Standardized and unified IO interface for all message types
 */

void opcommMessage::eraseMessage(){
    this->containerMessageReadable.clear();
    this->containerMessageEncrypted.clear();
    this->containerMessageEncryptedChecksum.clear();
}

void opcommMessage::printMessage(MessageType messageType){
    switch (messageType){
        case MessageReadable:
            Helper_PrintMessage(containerMessageReadable);
            break;
        case MessageEncrypted:
            Helper_PrintMessage(containerMessageEncrypted);
            break;
        case MessageEncryptedChecksum:
            Helper_PrintMessage(containerMessageEncryptedChecksum);
            break;
        default:
            std::cout << Helper_IOErrorMessage_UnrecognizedMessageType("printMessage") << std::endl;
            break;
    }
}

void opcommMessage::appendToMessageReable(const std::string &messageTrailing){
       lengthMessageReadable += Helper_AppendToMessage(messageTrailing, containerMessageReadable);
       getSyncFromMessageReadable();
}

void opcommMessage::replaceMessageReadableWith(const std::string &newMessage){
    lengthMessageReadable = Helper_ReplaceMessageWith(newMessage, containerMessageReadable);
    getSyncFromMessageReadable();
}

bool opcommMessage::writeMessageToFile(MessageType messageType, const std::string &outputFileName){
    bool result;
    switch (messageType){
        case MessageReadable:
            result = Helper_WriteMessageToFile(outputFileName, containerMessageReadable);
            break;
        case MessageEncrypted:
            result = Helper_WriteMessageToFile(outputFileName, containerMessageEncrypted);
            break;
        case MessageEncryptedChecksum:
            result = Helper_WriteMessageToFile(outputFileName, containerMessageEncryptedChecksum);
            break;
        default:
            std::cout << Helper_IOErrorMessage_UnrecognizedMessageType("writeMessageToFile") << std::endl;
            result = false;
            break;
    }
    return result;
}

bool opcommMessage::appendMessageToFile(MessageType messageType, const std::string &outputFileName){
    bool result;
    switch (messageType){
        case MessageReadable:
            result = Helper_AppendMessageToFile(outputFileName, containerMessageReadable);
            break;
        case MessageEncrypted:
            result = Helper_AppendMessageToFile(outputFileName, containerMessageEncrypted);
            break;
        case MessageEncryptedChecksum:
            result = Helper_AppendMessageToFile(outputFileName, containerMessageEncryptedChecksum);
            break;
        default:
            std::cout << Helper_IOErrorMessage_UnrecognizedMessageType("appendMessageToFile") << std::endl;
            result = false;
            break;
    }
    return result;
}

bool opcommMessage::readMessageFromFile_ReplacingExistingMessages(MessageType messageType, const std::string &inputFileName){
    bool result;
    switch (messageType){
        case MessageReadable:
            result = Helper_ReadMessageFromFile_ReplacingExistingMessages(inputFileName, containerMessageReadable, lengthMessageReadable);
            if (result)
                getSyncFromMessageReadable();
            break;
        case MessageEncrypted:
            result = Helper_ReadMessageFromFile_ReplacingExistingMessages(inputFileName, containerMessageEncrypted, lengthMessageEncrypted);
            if (result)
                getSyncFromMessageEncrypted();
            break;
        case MessageEncryptedChecksum:
            result = Helper_ReadMessageFromFile_ReplacingExistingMessages(inputFileName, containerMessageEncryptedChecksum, lengthMessageEncryptedChecksum);
            if (result)
                getSyncFromMessageEncryptedChecksum();
            break;
        default:
            result = false;
            std::cout << Helper_IOErrorMessage_UnrecognizedMessageType("readMessageFromFile_ReplacingExistingMessages") << std::endl;
            break;
    }
    return result;
}


bool opcommMessage::writeMessageBitPatternToFile(MessageType messageType, const std::string &outputFileName){
    bool result;
    switch (messageType){
        case MessageReadable:
            result = Helper_WriteBitPatternToFile(outputFileName, containerMessageReadable, lengthMessageReadable);
            break;
        case MessageEncrypted:
            result = Helper_WriteBitPatternToFile(outputFileName, containerMessageEncrypted, lengthMessageEncrypted);
            break;
        case MessageEncryptedChecksum:
            result = Helper_WriteBitPatternToFile(outputFileName, containerMessageEncryptedChecksum, lengthMessageEncryptedChecksum);
            break;
        default:
            std::cout << Helper_IOErrorMessage_UnrecognizedMessageType("writeMessageBitPatternToFile") << std::endl;
            result = false;
            break;
    }
    return result;
}

bool opcommMessage::appendMessageBitPatternToFile(MessageType messageType, const std::string &outputFileName){
    bool result;
    switch (messageType){
        case MessageReadable:
            result = Helper_AppendBitPatternToFile(outputFileName, containerMessageReadable, lengthMessageReadable);
            break;
        case MessageEncrypted:
            result = Helper_AppendBitPatternToFile(outputFileName, containerMessageEncrypted, lengthMessageEncrypted);
            break;
        case MessageEncryptedChecksum:
            result = Helper_AppendBitPatternToFile(outputFileName, containerMessageEncryptedChecksum, lengthMessageEncryptedChecksum);
            break;
        default:
            std::cout << Helper_IOErrorMessage_UnrecognizedMessageType("appendMessageBitPatternToFile") << std::endl;
            result = false;
            break;
    }
    return result;
}

bool opcommMessage::readMessageBitPatternFromFile_ReplacingExistingMessages(MessageType messageType, const std::string &inputFileName){
    bool result;
    switch (messageType){
        case MessageReadable:
            result = Helper_ReadBitPatternFromFile_ReplacingExistingMessages(inputFileName, containerMessageReadable, lengthMessageReadable);
            if (result)
                getSyncFromMessageReadable();
            break;
        case MessageEncrypted:
            result = Helper_ReadBitPatternFromFile_ReplacingExistingMessages(inputFileName, containerMessageEncrypted, lengthMessageEncrypted);
            if (result)
                getSyncFromMessageEncrypted();
            break;
        case MessageEncryptedChecksum:
            result = Helper_ReadBitPatternFromFile_ReplacingExistingMessages(inputFileName, containerMessageEncryptedChecksum, lengthMessageEncryptedChecksum);
            if (result)
                getSyncFromMessageEncryptedChecksum();
            break;
        default:
            result = false;
            std::cout << Helper_IOErrorMessage_UnrecognizedMessageType("readMessageBitPatternFromFile_ReplacingExistingMessages") << std::endl;
            break;
    }
    return result;
}


std::string opcommMessage::messageToString(MessageType messageType){
    switch (messageType){
        case MessageReadable:
            return Helper_toString(containerMessageReadable);
            break;
        case MessageEncrypted:
            return Helper_toString(containerMessageEncrypted);
            break;
        case MessageEncryptedChecksum:
            return Helper_toString(containerMessageEncryptedChecksum);
            break;
        default:
            std::cout << Helper_IOErrorMessage_UnrecognizedMessageType("messageToString") << std::endl;
            return "";
            break;
    }
}

std::string opcommMessage::messageToBitPattern(MessageType messageType){
    switch (messageType){
        case MessageReadable:
            return Helper_toBitPattern(containerMessageReadable, lengthMessageReadable);
            break;
        case MessageEncrypted:
            return Helper_toBitPattern(containerMessageEncrypted, lengthMessageEncrypted);
            break;
        case MessageEncryptedChecksum:
            return Helper_toBitPattern(containerMessageEncryptedChecksum, lengthMessageEncryptedChecksum);
            break;
        default:
            std::cout << Helper_IOErrorMessage_UnrecognizedMessageType("messageToBitPattern") << std::endl;
            return "";
            break;
    }
}

// Private Helper Functions
void opcommMessage::Helper_PrintMessage(const std::vector<unsigned char> &messageContainer){
    std::cout << Helper_toString(messageContainer);
}

unsigned int opcommMessage::Helper_AppendToMessage(const std::string &messageTrailing, std::vector<unsigned char> & messageContainer){
    // Adding new line characters
    messageContainer.push_back('\r');
    messageContainer.push_back('\n');
    copy(messageTrailing.begin(), messageTrailing.end(), back_inserter(messageContainer));
    return (messageTrailing.length() + 2) * CHAR_BIT;
}

unsigned int opcommMessage::Helper_ReplaceMessageWith(const std::string &newMessage, std::vector<unsigned char> &messageContainer){
    messageContainer.clear();
    copy(newMessage.begin(), newMessage.end(), back_inserter(messageContainer));
    return newMessage.length() * CHAR_BIT;
}

bool opcommMessage::Helper_ReadMessageFromFile_ReplacingExistingMessages(const std::string &inputFileName, std::vector<unsigned char> &messageContainer, unsigned int & messageLength){
    std::ifstream inputFile(inputFileName.c_str(), std::fstream::in);
    bool fileIsOpen = inputFile.is_open();
    if (fileIsOpen){
        eraseMessage();
        std::string line;
        unsigned int counter = 0;
        messageLength = 0;
        while (getline(inputFile, line)){
            if (counter == 0){
                messageLength += Helper_ReplaceMessageWith(line, messageContainer);
            } else {
                messageLength += Helper_AppendToMessage(line, messageContainer);
            }
            counter ++;
        }
    } else {
        std::cout << "Error: Unable to open " << "input" << " file " << inputFileName << "!" << std::endl;
    }
    inputFile.clear();
    inputFile.close();
    return fileIsOpen;
}

void opcommMessage::Helper_PublishMessageToFile(std::ofstream &outputFile, const std::vector<unsigned char> &messageContainer){
    outputFile << Helper_toString(messageContainer);
}

bool opcommMessage::Helper_AppendMessageToFile(const std::string &outputFileName, const std::vector<unsigned char> &messageContainer){
    std::ofstream outputFile(outputFileName.c_str(), std::fstream::out | std::fstream::app);
    bool fileIsOpen = outputFile.is_open();
    if (fileIsOpen){
        outputFile << '\r' << '\n';
        Helper_PublishMessageToFile(outputFile, messageContainer);
    } else {
        std::cout << "Error: Unable to open " << "output" << " file " << outputFileName << "!" << std::endl;
    }
    outputFile.clear();
    outputFile.close();
    return fileIsOpen;
}

bool opcommMessage::Helper_WriteMessageToFile(const std::string &outputFileName, const std::vector<unsigned char> &messageContainer){
    std::ofstream outputFile(outputFileName.c_str(), std::fstream::out | std::fstream::trunc);
    bool fileIsOpen = outputFile.is_open();
    if (fileIsOpen){
        Helper_PublishMessageToFile(outputFile, messageContainer);
    } else {
        std::cout << "Error: Unable to open " << "output"<< " file " << outputFileName << "!" << std::endl;
    }
    outputFile.clear();
    outputFile.close();
    return fileIsOpen;
}

bool opcommMessage::Helper_ReadBitPatternFromFile_ReplacingExistingMessages(const std::string &inputFileName, std::vector<unsigned char> &messageContainer, unsigned int & messageLength){
    std::ifstream inputFile(inputFileName.c_str(), std::fstream::in);
    bool fileIsOpen = inputFile.is_open();
    if (fileIsOpen){
        eraseMessage();
        std::string line;
        messageLength = 0;
        while (getline(inputFile,line)){
            unsigned short counter = 0;
            unsigned short totalLength = line.length();
            unsigned char character;
            messageLength += totalLength;
            while (totalLength - counter >= CHAR_BIT){
                character = convertBitToChar(line.substr(counter, CHAR_BIT));
                counter = counter + CHAR_BIT;
                messageContainer.push_back(character);
            }

            if (totalLength > counter){
                character = convertBitToChar(line.substr(counter));
                messageContainer.push_back(character);
            }
        }
    } else {
        std::cout << "Error: Unable to open " << "input" << " file " << inputFileName << "!" << std::endl;
    }
    inputFile.clear();
    inputFile.close();
    return fileIsOpen;
}

bool opcommMessage::Helper_AppendBitPatternToFile(const std::string &outputFileName, const std::vector<unsigned char> &messageContainer, const unsigned int messageLength){
    std::ofstream outputFile(outputFileName.c_str(), std::fstream::out | std::fstream::app);
    bool fileIsOpen = outputFile.is_open();
    if (fileIsOpen){
        outputFile << convertCharToBit('\r') << convertCharToBit('\n');
        outputFile << std::endl;
        Helper_PublishBitPatternToFile(outputFile, messageContainer, messageLength);
    } else {
        std::cout << "Error: Unable to open " << "output" << " file " << outputFileName << "!" << std::endl;
    }
    outputFile.clear();
    outputFile.close();
    return fileIsOpen;
}

bool opcommMessage::Helper_WriteBitPatternToFile(const std::string &outputFileName, const std::vector<unsigned char> &messageContainer, const unsigned int messageLength){
    std::ofstream outputFile(outputFileName.c_str(), std::fstream::out | std::fstream::trunc);
    bool fileIsOpen = outputFile.is_open();
    if (fileIsOpen){
        Helper_PublishBitPatternToFile(outputFile, messageContainer, messageLength);
    } else {
        std::cout << "Error: Unable to open " << "output" << " file " << outputFileName << "!" << std::endl;
    }
    outputFile.clear();
    outputFile.close();
    return fileIsOpen;
}

void opcommMessage::Helper_PublishBitPatternToFile(std::ofstream &outputFile, const std::vector<unsigned char> &messageContainer, const unsigned int messageLength){
    unsigned int counter = 0;
    for (std::vector<unsigned char>::const_iterator it = messageContainer.begin(); it != messageContainer.end(); ++ it){
        if ((counter > phraseLength) && (counter % phraseLength == 0)){
            outputFile << std::endl;
        }
        std::string charBitPattern = convertCharToBit(*it);
        if (counter * CHAR_BIT + CHAR_BIT <= messageLength){
            outputFile << charBitPattern;
        } else{
            for (unsigned int i = 0; i < messageLength % CHAR_BIT; i ++){
                outputFile << charBitPattern[i];
            }
        }
    }
}

std::string opcommMessage::Helper_toString(const std::vector<unsigned char> &messageContainer){
    std::string messageString(messageContainer.begin(), messageContainer.end());
    return messageString;
}

std::string opcommMessage::Helper_toBitPattern(const std::vector<unsigned char> &messageContainer, const unsigned int messageLength){
    std::string bitPattern;
    unsigned int counter = 0;
    for (std::vector<unsigned char>::const_iterator it = messageContainer.begin(); it != messageContainer.end(); ++ it){
        std::string charBitPattern = convertCharToBit(*it);
        if (counter * CHAR_BIT + CHAR_BIT <= messageLength){
            bitPattern += charBitPattern;
        } else{
            for (unsigned int i = 0; i < messageLength % CHAR_BIT; i ++){
                bitPattern += charBitPattern[i];
            }
        }
        counter ++;
    }
    return bitPattern;
}

std::string opcommMessage::convertCharToBit(unsigned char ch){
    std::string bitPattern = "";
    for (size_t i = 0; i < CHAR_BIT; i++){
        bitPattern += (ch & mask[i])? '1': '0';
    }
    return bitPattern;
}

// Length of the bitPattern String needs to be less or equal than CHAR_BIT (8)
unsigned char opcommMessage::convertBitToChar(const std::string &bitPattern){
    assert(bitPattern.length() <= CHAR_BIT);

    unsigned char character = 0;
    for (size_t  i = 0; i < bitPattern.length(); i ++)
        character += (bitPattern[i] == '1')? mask[i]: 0;
    return character;
}

void opcommMessage::getSyncFromMessageReadable(){
    // Sequential Calls -- Order matters
    // Do NOT parallelize
    lengthMessageEncrypted = encryptMessage();
    lengthMessageEncryptedChecksum = addCheckSumOnEncrytedMessage();
}

void opcommMessage::getSyncFromMessageEncrypted(){
    // Interchangagable Steps
    // Can be parallelize if needed
    lengthMessageReadable = decryptMessage();
    lengthMessageEncryptedChecksum = addCheckSumOnEncrytedMessage();
}

void opcommMessage::getSyncFromMessageEncryptedChecksum(){
    // Sequential Calls -- Order matters
    // Do NOT parallelize
    lengthMessageEncrypted = removeCheckSumOnEncrytedMessage();
    lengthMessageReadable = decryptMessage();
}

unsigned int opcommMessage::encryptMessage(){
    switch (this->encryption){
        case EncryptionMethodNone:
            Helper_ReplaceMessageWith(opcommEncryptionMethods::encrypt_NoEncryptionMethod(messageToString(opcommMessage::MessageReadable)), this->containerMessageEncrypted);
            return lengthMessageReadable;
            break;
        default:
            Helper_ReplaceMessageWith(opcommEncryptionMethods::encrypt_NoEncryptionMethod(messageToString(opcommMessage::MessageReadable)), this->containerMessageEncrypted);
            return lengthMessageReadable;
            break;
    }
}

unsigned int opcommMessage::decryptMessage(){
    switch (this->encryption){
        case EncryptionMethodNone:
            Helper_ReplaceMessageWith(opcommEncryptionMethods::decipher_NoEncryptionMedthod(messageToString(opcommMessage::MessageEncrypted)), this->containerMessageReadable);
            return lengthMessageEncrypted;
            break;
        default:
            Helper_ReplaceMessageWith(opcommEncryptionMethods::decipher_NoEncryptionMedthod(messageToString(opcommMessage::MessageEncrypted)), this->containerMessageReadable);
            return lengthMessageEncrypted;
            break;
    }
}

unsigned int opcommMessage::addCheckSumOnEncrytedMessage(){
    switch (this->checksum){
        case ChecksumMethodNone:
            Helper_ReplaceMessageWith(opcommChecksumMethods::addChecksum_NoChecksumMethod(messageToString(opcommMessage::MessageEncrypted)), this->containerMessageEncryptedChecksum);
            return lengthMessageEncrypted;
            break;
        default:
            Helper_ReplaceMessageWith(opcommChecksumMethods::addChecksum_NoChecksumMethod(messageToString(opcommMessage::MessageEncrypted)), this->containerMessageEncryptedChecksum);
            return lengthMessageEncrypted;
            break;
    }
}

unsigned int opcommMessage::removeCheckSumOnEncrytedMessage(){
    switch (this->checksum){
        case ChecksumMethodNone:
            Helper_ReplaceMessageWith(opcommChecksumMethods::rmChecksum_NoChecksumMethod(messageToString(opcommMessage::MessageEncryptedChecksum)), this->containerMessageEncrypted);
            return lengthMessageEncryptedChecksum;
            break;
        default:
            Helper_ReplaceMessageWith(opcommChecksumMethods::rmChecksum_NoChecksumMethod(messageToString(opcommMessage::MessageEncryptedChecksum)), this->containerMessageEncrypted);
            return lengthMessageEncryptedChecksum;
            break;
    }
}

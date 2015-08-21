#include <iostream>
#include <assert.h>
#include "opcommmessage.h"

using namespace std;

static const string textFile = "test.bitpattern";
static const string messageFile = "test.message";

void testOpcommMessage();

// Tests
int main() {

    testOpcommMessage();

    std::cout << "----------------------" << std::endl << std::endl;

    opcommMessage testFour;
    testFour.replaceMessageReadableWith("1");

    opcommMessage testFive;
    testFive.replaceMessageReadableWith("9");

    opcommMessage testSix;
    unsigned char testChar = '1' ^ '9';
    std::string testString;
    testString += testChar;
    testSix.replaceMessageReadableWith(testString);

    std::cout << testFour.messageToBitPattern(opcommMessage::MessageReadable) << std::endl;
    std::cout << testFive.messageToBitPattern(opcommMessage::MessageReadable) << std::endl;
    std::cout << testSix.messageToBitPattern(opcommMessage::MessageReadable) << std::endl;

    std::cout << std::endl << std::endl;

    opcommMessage testSeven;
    testSeven.replaceMessageReadableWith("my name is charlie");
    std::cout << testSeven.messageToBitPattern(opcommMessage::MessageReadable) << std::endl;
    testSeven.replaceMessageReadableWith(opcommChecksumMethods::addChecksum_LRC8(testSeven.messageToString(opcommMessage::MessageReadable)));
    std::cout << testSeven.messageToBitPattern(opcommMessage::MessageReadable) << std::endl;

    std::cout << opcommChecksumMethods::rmChecksum_LRC8(testSeven.messageToString(opcommMessage::MessageReadable)) << std::endl;
    std::cout << opcommChecksumMethods::validateChecksum_LRC8(testSeven.messageToString(opcommMessage::MessageReadable)) << std::endl;
    std::cout << opcommChecksumMethods::validateChecksum_LRC8("bb") << std::endl;
    return 0;

}

void testOpcommMessage(){
    opcommMessage test(opcommMessage::EncryptionMethodNone, opcommMessage::ChecksumMethodNone);
    test.replaceMessageReadableWith("Hello");
    test.printMessage(opcommMessage::MessageEncrypted);
    cout << endl << "--------------------" << endl;
    test.printMessage(opcommMessage::MessageEncryptedChecksum);
    cout << endl << "-------------------" << endl;
    test.replaceMessageReadableWith("Bye");
    cout << test.messageToString(opcommMessage::MessageEncrypted) << " : " << test.messageToBitPattern(opcommMessage::MessageEncryptedChecksum) << endl << "----------------" << endl;
    test.writeMessageToFile(opcommMessage::MessageEncrypted, messageFile);
    test.eraseMessage();
    cout << "erase messages : " << test.messageToString(opcommMessage::MessageEncrypted) << endl << "-----------" << endl;
    test.readMessageFromFile_ReplacingExistingMessages(opcommMessage::MessageEncryptedChecksum, messageFile);
    test.printMessage(opcommMessage::MessageEncrypted);
    cout << endl << "-------------" << endl;
    test.appendToMessageReable("Hello");
    test.printMessage(opcommMessage::MessageEncrypted);
    cout << endl << " ------------ " <<  endl;
    test.writeMessageBitPatternToFile(opcommMessage::MessageEncrypted, textFile);
    test.eraseMessage();
    cout << "erase messages : " << test.messageToString(opcommMessage::MessageEncrypted) << endl <<"-------------" << endl;
    test.readMessageBitPatternFromFile_ReplacingExistingMessages(opcommMessage::MessageEncryptedChecksum, textFile);
    test.printMessage(opcommMessage::MessageEncrypted);
    test.writeMessageToFile(opcommMessage::MessageEncryptedChecksum, messageFile);
    cout << endl <<"-------------" << endl;

    test.eraseMessage();
    test.readMessageFromFile_ReplacingExistingMessages(opcommMessage::MessageEncrypted, messageFile);
    test.printMessage(opcommMessage::MessageEncrypted);
    cout << endl;

    opcommMessage testTwo;
    test.eraseMessage();
    test.replaceMessageReadableWith("Hello");
    testTwo.replaceMessageReadableWith("Hello");

    assert (test == testTwo);
    cout << (test != testTwo) << endl;
    assert (false == (test != testTwo));

    testTwo.appendToMessageReable("Bye");
    assert (test != testTwo);

    opcommMessage testThree = testTwo;
    std::cout << "TestTwo : " << testTwo.messageToString(opcommMessage::MessageReadable) << std::endl;
    std::cout << "TestThree : " << testThree.messageToString(opcommMessage::MessageReadable) << std::endl;
}

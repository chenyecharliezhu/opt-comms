#include <iostream>
#include <assert.h>
#include "opcommmessage.h"

using namespace std;

static const string textFile = "test.bitpattern";
static const string messageFile = "test.message";

void testOpcommMessage();

// Tests
int main() {

    opcommMessage testFour;
    testFour.replaceMessageWith(opcommMessage::MessageReadable, "1");

    opcommMessage testFive;
    testFive.replaceMessageWith(opcommMessage::MessageReadable, "9");

    opcommMessage testSix;
    unsigned char testChar = '1' ^ '6';
    std::string testString;
    testString += testChar;
    testSix.replaceMessageWith(opcommMessage::MessageReadable, testString);

    std::cout << testFour.messageToBitPattern(opcommMessage::MessageReadable) << std::endl;
    std::cout << testFive.messageToBitPattern(opcommMessage::MessageReadable) << std::endl;
    std::cout << testSix.messageToBitPattern(opcommMessage::MessageReadable) << std::endl;

    std::cout << std::endl << std::endl;

    opcommMessage testSeven;
    testSeven.replaceMessageWith(opcommMessage::MessageReadable, "my name is charlie");
    std::cout << testSeven.messageToBitPattern(opcommMessage::MessageReadable) << std::endl;
    testSeven.replaceMessageWith(opcommMessage::MessageReadable, opcommChecksumMethods::addChecksum_LRC8(testSeven.messageToString(opcommMessage::MessageReadable)));
    std::cout << testSeven.messageToBitPattern(opcommMessage::MessageReadable) << std::endl;

    std::cout << opcommChecksumMethods::rmChecksum_LRC8(]testSeven.messageToString(opcommMessage::MessageReadable)) << std::endl;
    std::cout << opcommChecksumMethods::validateChecksum_LRC8(testSeven.messageToString(opcommMessage::MessageReadable)) << std::endl;
    std::cout << opcommChecksumMethods::validateChecksum_LRC8("bb") << std::endl;
    return 0;

}

void testOpcommMessage(){
    opcommMessage test(opcommMessage::EncryptionMethodNone, opcommMessage::ChecksumMethodNone);
    test.replaceMessageWith(opcommMessage::MessageReadable, "Hello");
    test.printMessage(opcommMessage::MessageEncrypted);
    cout << endl << "--------------------" << endl;
    test.printMessage(opcommMessage::MessageEncryptedChecksum);
    cout << endl << "-------------------" << endl;
    test.replaceMessageWith(opcommMessage::MessageEncrypted, "Bye");
    cout << test.messageToString(opcommMessage::MessageEncrypted) << " : " << test.messageToBitPattern(opcommMessage::MessageEncryptedChecksum) << endl << "----------------" << endl;
    test.writeMessageToFile(opcommMessage::MessageEncrypted, messageFile);
    test.eraseMessage();
    cout << "erase messages : " << test.messageToString(opcommMessage::MessageEncrypted) << endl << "-----------" << endl;
    test.readMessageFromFile_ReplacingExistingMessages(opcommMessage::MessageEncryptedChecksum, messageFile);
    test.printMessage(opcommMessage::MessageEncrypted);
    cout << endl << "-------------" << endl;
    test.appendToMessage(opcommMessage::MessageReadable, "Hello");
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
    test.replaceMessageWith(opcommMessage::MessageReadable, "Hello");
    testTwo.replaceMessageWith(opcommMessage::MessageReadable, "Hello");

    assert (test == testTwo);
    cout << (test != testTwo) << endl;
    assert (false == (test != testTwo));

    testTwo.appendToMessage(opcommMessage::MessageEncrypted, "Bye");
    assert (test != testTwo);

    opcommMessage testThree = testTwo;
    std::cout << "TestTwo : " << testTwo.messageToString(opcommMessage::MessageReadable) << std::endl;
    std::cout << "TestThree : " << testThree.messageToString(opcommMessage::MessageReadable) << std::endl;
}

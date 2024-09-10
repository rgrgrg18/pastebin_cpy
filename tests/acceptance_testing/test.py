import unittest
import json
import grpc
import server_pb2_grpc
import server_pb2

def load_config():
    with open('config.json', 'r') as f:
        return json.load(f)

# Load config
config = load_config()
server_adress = config['server_adress']

# Custom TestResult class to handle custom output
class CustomTestResult(unittest.TextTestResult):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.successes = []  # Add a list to track successes

    def startTest(self, test):
        super().startTest(test)
        print(f"\nStarting test: {test._testMethodName}")

    def addSuccess(self, test):
        super().addSuccess(test)
        self.successes.append(test)  # Track success
        print(f"✅ {test._testMethodName} passed")

    def addFailure(self, test, err):
        super().addFailure(test, err)
        print(f"❌ {test._testMethodName} failed")

    def addError(self, test, err):
        super().addError(test, err)
        print(f"⚠️ {test._testMethodName} encountered an error")

    def addSkip(self, test, reason):
        super().addSkip(test, reason)
        print(f"⏭️ {test._testMethodName} skipped: {reason}")

# Custom TestRunner to use our CustomTestResult
class CustomTestRunner(unittest.TextTestRunner):
    def _makeResult(self):
        return CustomTestResult(self.stream, self.descriptions, self.verbosity)

    def run(self, test):
        result = super().run(test)
        # Custom conclusion
        print("\n==== Custom Test Suite Summary ====")
        print(f"Total tests run: {result.testsRun}")
        print(f"Successes: {len(result.successes)}")
        print(f"Failures: {len(result.failures)}")
        print(f"Errors: {len(result.errors)}")
        print("===================================")
        return result

class PastebinApiTest(unittest.TestCase):
    def setUp(self):
        self.channel = grpc.insecure_channel(server_adress)
        self.stub = server_pb2_grpc.pastebinApiStub(self.channel)

        request = server_pb2.newPasteArgs(user_id=1, text="Hello World", title="Test Paste", password="secret")
        response = self.stub.makeNewPaste(request)
        self.protected_public_key = response.public_key

        request = server_pb2.newPasteArgs(user_id=1, text="Hello World", title="Test Paste")
        response = self.stub.makeNewPaste(request)
        self.unsafety_public_key = response.public_key


    def test_make_new_paste(self):
        self.assertIsNotNone(self.protected_public_key)
        self.assertIsNotNone(self.unsafety_public_key)

    def test_get_paste_with_password(self):
        request = server_pb2.getPasteArgs(public_key=self.protected_public_key, password="secret")
        response = self.stub.getPaste(request)

        self.assertEqual(response.paste_text, "Hello World")
        self.assertEqual(response.title, "Test Paste")
        self.assertEqual(response.password, "secret")

        request = server_pb2.getPasteArgs(public_key=self.protected_public_key, password="random_password")
        with self.assertRaises(grpc.RpcError):
            self.stub.getPaste(request)

    def test_get_paste_without_password(self):
        request = server_pb2.getPasteArgs(public_key=self.unsafety_public_key, password="")
        response = self.stub.getPaste(request)

        self.assertEqual(response.paste_text, "Hello World")
        self.assertEqual(response.title, "Test Paste")
        self.assertEqual(response.password, "")

        request = server_pb2.getPasteArgs(public_key=self.unsafety_public_key, password="random_password")
        with self.assertRaises(grpc.RpcError):
            self.stub.getPaste(request)

    def test_update_paste(self):
        request = server_pb2.updatePasteArgs(public_key=self.protected_public_key, new_title="Updated Title", new_password="newsecret")
        response = self.stub.updatePaste(request)

        self.assertTrue(response.is_success)

        get_request = server_pb2.getPasteArgs(public_key=self.protected_public_key, password="newsecret")
        updated_paste = self.stub.getPaste(get_request)

        self.assertEqual(updated_paste.title, "Updated Title")
        self.assertEqual(updated_paste.password, "newsecret")

    def change_access(self):
        request = server_pb2.updatePasteArgs(public_key=self.unsafety_public_key, new_title="Updated Title", new_password="newsecret")
        response = self.stub.updatePaste(request)

        self.assertTrue(response.is_success)

        request = server_pb2.getPasteArgs(public_key=self.unsafety_public_key, password="")
        with self.assertRaises(grpc.RpcError):
            self.stub.getPaste(request)


    def tearDown(self):
        request = server_pb2.delPasteArgs(public_key=self.protected_public_key)
        response = self.stub.deletePaste(request)

        self.assertTrue(response.is_success)

        request = server_pb2.delPasteArgs(public_key=self.unsafety_public_key)
        response = self.stub.deletePaste(request)

        self.assertTrue(response.is_success)

        self.channel.close()

if __name__ == '__main__':
    # Run tests with the custom test runner
    runner = CustomTestRunner(verbosity=0)
    unittest.main(testRunner=runner)
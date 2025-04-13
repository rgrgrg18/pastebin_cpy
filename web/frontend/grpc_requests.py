import grpc
import server_pb2
import server_pb2_grpc

def make_new_paste(channel, user_id, text, title=None, password=None):
    stub = server_pb2_grpc.pastebinApiStub(channel)
    request = server_pb2.newPasteArgs(
        user_id=user_id,
        text=text,
        title=title,
        password=password
    )
    response = stub.makeNewPaste(request)
    return response.public_key

def get_paste(channel, public_key, password=None):
    stub = server_pb2_grpc.pastebinApiStub(channel)
    request = server_pb2.getPasteArgs(
        public_key=public_key,
        password=password
    )
    response = stub.getPaste(request)
    return {
        "author": response.author,
        "password": response.password,
        "title": response.title,
        "created_at": response.created_at,
        "content": response.paste_text,
        "language": "python"
    }
from flask import Flask, render_template, request, jsonify
import os
from datetime import datetime
import uuid
import grpc
import server_pb2
import server_pb2_grpc

from grpc_requests import make_new_paste, get_paste

app = Flask(__name__, 
            template_folder=os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates'))

def get_grpc_channel():
    return grpc.insecure_channel('') # set address of your gRPC server

@app.route('/paste/<paste_id>')
def view_paste(paste_id):
    try:
        with get_grpc_channel() as channel:
            paste = get_paste(channel, paste_id, password=None)

        full_text = paste['content']
        lines = full_text.split('\n', 1)
        language = lines[0]
        paste_text = lines[1] if len(lines) > 1 else ''

        paste['language'] = language
        paste['content'] = paste_text

        current_year = datetime.now().year
        return render_template('view_paste.html', paste=paste, current_year=current_year)
    except Exception as e:
        return jsonify({'error': str(e)}), 500


@app.route('/paste/create', methods=['GET', 'POST'])
def create_paste():
    if request.method == 'POST':
        try:
            data = request.get_json()

            if not data or 'content' not in data:
                return jsonify({'error': 'Content is required'}), 400
            
            language = data.get('language', 'plain text')
            
            text = f"{language}\n{data['content']}"
            
            with get_grpc_channel() as channel:
                user_id = 1

                public_key = make_new_paste(
                    channel=channel,
                    user_id=user_id,
                    text=text,
                    title=data.get('title', 'Untitled'),
                    password=data.get('password')
                )

            return jsonify({
                'id': public_key,
                'message': 'Paste created successfully'
            }), 201
        except Exception as e:
            return jsonify({'error': str(e)}), 500
    
    return render_template('create_paste.html')

if __name__ == '__main__':
    app.run(debug=True, port=5000)

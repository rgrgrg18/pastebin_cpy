from flask import Flask, render_template, request, redirect, url_for, jsonify
import os
from datetime import datetime
import requests
import uuid

app = Flask(__name__, 
            template_folder=os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates'))

pastes = {}

@app.route('/paste/<paste_id>')
def view_paste(paste_id):
    try:
        if paste_id not in pastes:
            return jsonify({'error': 'Paste not found'}), 404

        paste = pastes[paste_id]
        current_year = datetime.now().year
        return render_template('view_paste.html', paste=paste, current_year=current_year)
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/paste/create', methods=['GET', 'POST'])
def create_paste():
    if request.method == 'POST':
        try:
            data = request.get_json()
            
            # Validate required fields
            if not data or 'content' not in data:
                return jsonify({'error': 'Content is required'}), 400
                
            # Generate unique ID
            paste_id = str(uuid.uuid4())
            
            # Create paste object
            paste = {
                'id': paste_id,
                'content': data['content'],
                'title': data.get('title', 'Untitled'),
                'language': data.get('language', 'text'),
                'created_at': datetime.now(),
                'last_edit': datetime.now(),
            }
            
            # Store paste
            pastes[paste_id] = paste
            
            return jsonify({
                'id': paste_id,
                'message': 'Paste created successfully'
            }), 201
        
        except Exception as e:
            return jsonify({'error': str(e)}), 500
    
    return render_template('create_paste.html')

if __name__ == '__main__':
    app.run(debug=True, port=5000) 
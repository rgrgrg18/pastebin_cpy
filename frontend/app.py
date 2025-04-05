from flask import Flask, render_template
import os
from datetime import datetime

app = Flask(__name__, 
            template_folder=os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates'))

# Mock data for testing
MOCK_PASTES = {
    "1": {
        "content": "This is a sample paste content.\nIt can contain multiple lines.\nAnd even code snippets:\n\ndef hello_world():\n    print('Hello, World!')",
        "title": "Sample Paste",
        "language": "python",
        "created_at": datetime(2025, 4, 5, 15, 30, 0),
        "last_edit": datetime(2025, 4, 5, 16, 0, 0)
    }
}

@app.route('/paste/<paste_id>')
def view_paste(paste_id):
    # For now, we'll use mock data
    paste = MOCK_PASTES.get(paste_id)
    if not paste:
        return "Paste not found", 404
    
    current_year = datetime.now().year
    return render_template('view_paste.html', paste=paste, current_year=current_year)

if __name__ == '__main__':
    app.run(debug=True, port=5000) 
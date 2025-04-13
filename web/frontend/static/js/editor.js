// Editor configuration
const editor = document.getElementById('paste-content');
const languageSelect = document.getElementById('language-select');
const fontSizeSelect = document.getElementById('font-size-select');
const themeToggle = document.getElementById('theme-toggle');
const previewSection = document.getElementById('preview-section');
const previewContent = document.querySelector('.preview-content');
const editorSection = document.querySelector('.editor-section');
const resizer = document.querySelector('.resizer');
let isResizing = false;
let lastX = 0;

// Initialize CodeMirror
const codeMirror = CodeMirror.fromTextArea(editor, {
    lineNumbers: true,
    mode: 'python',
    theme: 'monokai',
    indentUnit: 4,
    lineWrapping: true,
    autoCloseBrackets: true,
    matchBrackets: true,
    indentWithTabs: false,
    smartIndent: true,
    electricChars: true,
    extraKeys: {
        "Tab": function(cm) {
            const spaces = Array(cm.getOption("indentUnit") + 1).join(" ");
            cm.replaceSelection(spaces);
        }
    }
});

// Language mode mapping with more specific modes
const languageModes = {
    'python': {
        mode: 'python'
    },
    'cpp': {
        mode: 'text/x-c++src'
    },
    'sql': 'text/x-sql',
    'markdown': 'markdown'
};

function setupResizer() {
    if (languageSelect.value === 'markdown') {
        editorSection.style.flex = '1 1 50%';
        previewSection.style.flex = '1 1 50%';
        resizer.style.display = 'block';
    } else {
        editorSection.style.flex = '1 1 100%';
        previewSection.style.flex = '0 0 0';
        resizer.style.display = 'none';
    }
}

resizer.addEventListener('mousedown', (e) => {
    if (languageSelect.value !== 'markdown') return;
    isResizing = true;
    lastX = e.clientX;
    document.body.style.cursor = 'col-resize';
    editorSection.style.userSelect = 'none';
    previewSection.style.userSelect = 'none';
});

document.addEventListener('mousemove', (e) => {
    if (!isResizing || languageSelect.value !== 'markdown') return;

    const container = editorSection.parentElement;
    const containerWidth = container.offsetWidth;
    const dx = e.clientX - lastX;

    const editorWidth = editorSection.offsetWidth;
    const newWidth = editorWidth + dx;
    const minWidth = 200; // Minimum width for each section

    if (newWidth >= minWidth && (containerWidth - newWidth) >= minWidth) {
        editorSection.style.flex = `0 0 ${newWidth}px`;
        previewSection.style.flex = `1 1 ${containerWidth - newWidth}px`;
        lastX = e.clientX;
    }
});

document.addEventListener('mouseup', () => {
    if (!isResizing) return;

    isResizing = false;
    document.body.style.cursor = '';
    editorSection.style.userSelect = '';
    previewSection.style.userSelect = '';
});

// Update resizer when language changes
languageSelect.addEventListener('change', function() {
    const modeConfig = languageModes[this.value];
    codeMirror.setOption('mode', modeConfig.mode);

    if (this.value === 'markdown') {
        previewSection.classList.add('markdown');
        updatePreview();
    } else {
        previewSection.classList.remove('markdown');
    }

    setupResizer();
});

// Update preview content
function updatePreview() {
    if (languageSelect.value === 'markdown') {
        const content = codeMirror.getValue();
        previewContent.innerHTML = marked.parse(content);
    }
}

// Listen for changes in the editor
codeMirror.on('change', function() {
    if (languageSelect.value === 'markdown') {
        updatePreview();
    }
});

// Font size handler
fontSizeSelect.addEventListener('change', function() {
    const fontSize = this.value + 'px';
    codeMirror.getWrapperElement().style.fontSize = fontSize;
    previewContent.style.fontSize = fontSize;
});

// Theme handling
function initTheme() {
    const savedTheme = localStorage.getItem('theme') || 'light';
    document.body.classList.toggle('dark-theme', savedTheme === 'dark');
    updateThemeIcon();
    updateEditorTheme();
}

function toggleTheme() {
    document.body.classList.toggle('dark-theme');
    const isDark = document.body.classList.contains('dark-theme');
    localStorage.setItem('theme', isDark ? 'dark' : 'light');
    updateThemeIcon();
    updateEditorTheme();
}

function updateThemeIcon() {
    const isDark = document.body.classList.contains('dark-theme');
    themeToggle.innerHTML = isDark ?
        `<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <path d="M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z"></path>
        </svg>` :
        `<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <circle cx="12" cy="12" r="5"/>
            <line x1="12" y1="1" x2="12" y2="3"/>
            <line x1="12" y1="21" x2="12" y2="23"/>
            <line x1="4.22" y1="4.22" x2="5.64" y2="5.64"/>
            <line x1="18.36" y1="18.36" x2="19.78" y2="19.78"/>
            <line x1="1" y1="12" x2="3" y2="12"/>
            <line x1="21" y1="12" x2="23" y2="12"/>
            <line x1="4.22" y1="19.78" x2="5.64" y2="18.36"/>
            <line x1="18.36" y1="5.64" x2="19.78" y2="4.22"/>
        </svg>`;
}

function updateEditorTheme() {
    const isDark = document.body.classList.contains('dark-theme');
    codeMirror.setOption('theme', isDark ? 'monokai' : 'default');
}

// Initialize theme
initTheme();
themeToggle.addEventListener('click', toggleTheme);

// Form submission handler
document.getElementById('paste-form').addEventListener('submit', e => {
    e.preventDefault(); // Prevent default form submission

    const title = document.getElementById('paste-title').value;
    const content = codeMirror.getValue();
    const language = languageSelect.value;

    const pasteData = {
        title: title,
        content: content,
        language: language
    };

    fetch(`/paste/create`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(pasteData)
    })
    .then(response => response.json())
    .then(data => {
        if (data.error) {
            alert('Error: ' + data.error);
        } else {
            alert('Paste created successfully!');
            window.location.href = '/paste/' + data.id;
        }
    })
    .catch(error => {
        alert('An error occurred: ' + error.message);
    });
});

// Initialize resizer
setupResizer();

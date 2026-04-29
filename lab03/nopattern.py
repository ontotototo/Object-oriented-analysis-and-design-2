from flask import Flask, jsonify, request, send_from_directory, send_file
from flask_cors import CORS
import os

app = Flask(__name__, static_folder="static")
CORS(app)

class MediaFile:
    def __init__(self, title: str, m_type: str, size: str, preview: str = ""):
        self.title = title
        self.m_type = m_type
        self.size = size
        self.preview = preview 

    def to_dict(self):
        return {
            "title": self.title,
            "type": self.m_type,
            "size": self.size,
            "preview": self.preview
        }

all_files = [
    MediaFile("Танцевальный", "image", "2.4 MB", "tanec26.jpg"),
    MediaFile("Массовый", "video", "15.8 MB", "видео.jpg"),
    MediaFile("Курсач", "doc", "7 МB", "doc.jpg"),
    MediaFile("Геленджик", "image", "3.1 MB", "gel.jpg"),
    MediaFile("Паттерны книга", "doc", "37.1 MB", "pdf.jpg")
]
# Теперь хранить эти переменные прямо в памяти сервера
current_filtered_files = all_files[:] # Список файлов после фильтрации
current_index = 0                     # Текущая позиция
library_name = "Галерея (Без паттерна)"

def get_current_state():
    # Логика определения "есть ли следующий" теперь вычисляется здесь каждый раз
    file = current_filtered_files[current_index] if current_filtered_files else None
    return {
        "file": file.to_dict() if file else None,
        "index": current_index,
        "total": len(current_filtered_files),
        "has_next": current_index < len(current_filtered_files) - 1,
        "has_previous": current_index > 0,
        "library_name": library_name,
        "all_files": [f.to_dict() for f in all_files]
    }


@app.route("/")
def index():
    return send_file("static/index.html")

@app.route("/api/state")
def api_state():
    return jsonify(get_current_state())

@app.route("/api/next", methods=["POST"])
def api_next():
    global current_index
    # Логика перемещения "зашита" прямо в эндпоинт
    if current_index < len(current_filtered_files) - 1:
        current_index += 1
    return jsonify(get_current_state())

@app.route("/api/previous", methods=["POST"])
def api_previous():
    global current_index
    if current_index > 0:
        current_index -= 1
    return jsonify(get_current_state())

@app.route("/api/filter/<m_type>", methods=["POST"])
def api_filter(m_type):
    global current_filtered_files, current_index
    
    # Логика фильтрации  выполняется прямо здесь
    if m_type == "all":
        current_filtered_files = all_files[:]
    else:
        current_filtered_files = [f for f in all_files if f.m_type == m_type]
    
    current_index = 0 # Сброс позиции при каждой смене фильтра
    return jsonify(get_current_state())

PREVIEWS_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "covers")

@app.route("/previews/<filename>")
def serve_preview(filename):
    return send_from_directory(PREVIEWS_DIR, filename)

if __name__ == "__main__":
    print("ЗАПУСК: Версия БЕЗ паттерна Итератор")
    app.run(debug=True, port=5001, host='0.0.0.0')
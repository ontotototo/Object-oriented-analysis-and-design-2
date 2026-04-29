from flask import Flask, jsonify, request, send_from_directory, send_file
from flask_cors import CORS
from abc import ABC, abstractmethod
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

class MediaIterator(ABC):
    @abstractmethod
    def has_next(self) -> bool: pass

    @abstractmethod
    def next(self) -> MediaFile: pass

    @abstractmethod
    def has_previous(self) -> bool: pass

    @abstractmethod
    def previous(self) -> MediaFile: pass

    @abstractmethod
    def current(self) -> MediaFile: pass

    @abstractmethod
    def current_index(self) -> int: pass

    @abstractmethod
    def total(self) -> int: pass

class FileIterator(MediaIterator):
    def __init__(self, all_files: list[MediaFile], filter_type: str):
        
        if filter_type == "all":
            self.files = all_files
        else:
            self.files = [f for f in all_files if f.m_type == filter_type]
        
        self.index = 0 

    def has_next(self) -> bool:
        return self.index < len(self.files) - 1

    def next(self) -> MediaFile:
        if self.has_next():
            self.index += 1
        return self.current()

    def has_previous(self) -> bool:
        return self.index > 0

    def previous(self) -> MediaFile:
        if self.has_previous():
            self.index -= 1
        return self.current()

    def current(self) -> MediaFile:
        return self.files[self.index] if self.files else None

    def current_index(self) -> int:
        return self.index

    def total(self) -> int:
        return len(self.files)
    
class MediaCollection(ABC):
    @abstractmethod
    def create_iterator(self, filter: str) -> MediaIterator: pass

class MediaLibrary(MediaCollection):
    def __init__(self, name: str):
        self.name = name
        self.files: list[MediaFile] = []

    def create_iterator(self, filter: str) -> MediaIterator:
        # filter: "all", "image", "video", "doc"
        return FileIterator(self.files, filter)

    def add_file(self, file: MediaFile):
        self.files.append(file)

    def get_all_files(self):
        return self.files
    
PREVIEWS_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "covers")
os.makedirs(PREVIEWS_DIR, exist_ok=True)

library = MediaLibrary("Мои файлы")
library.add_file(MediaFile("Танцевальный", "image", "2.4 MB", "tanec26.jpg"))
library.add_file(MediaFile("Массовый", "video", "15.8 MB", "видео.jpg"))
library.add_file(MediaFile("Курсач", "doc", "7 МB", "doc.jpg"))
library.add_file(MediaFile("Геленджик", "image", "3.1 MB", "gel.jpg"))
library.add_file(MediaFile("Паттерны книга", "doc", "37.1 MB", "pdf.jpg"))

current_iterator = library.create_iterator("all")

def get_state():
    file = current_iterator.current()
    return {
        "file": file.to_dict() if file else None,
        "index": current_iterator.current_index(),
        "total": current_iterator.total(),
        "has_next": current_iterator.has_next(),
        "has_previous": current_iterator.has_previous(),
        "library_name": library.name,
        "filtered_files": [f.to_dict() for f in current_iterator.files] 
    }

@app.route("/")
def index():
    return send_file("static/index.html")

@app.route("/api/state")
def api_state():
    return jsonify(get_state())

@app.route("/api/next", methods=["POST"])
def api_next():
    current_iterator.next()
    return jsonify(get_state())

@app.route("/api/previous", methods=["POST"])
def api_previous():
    current_iterator.previous()
    return jsonify(get_state())

@app.route("/api/filter/<m_type>", methods=["POST"])
def api_filter(m_type):
    global current_iterator
    current_iterator = library.create_iterator(m_type)
    return jsonify(get_state())

@app.route("/previews/<filename>")
def serve_preview(filename):
    return send_from_directory(PREVIEWS_DIR, filename)

if __name__ == "__main__":
    app.run(debug=True, port=5001, host='0.0.0.0')
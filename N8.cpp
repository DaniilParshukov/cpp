#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>


class BoxWithSource {
public:
    ~BoxWithSource() {
        this->close();
    }

    virtual bool eof() = 0;
    virtual bool isOpen() = 0;
    virtual void close() = 0;
};

class Reader: BoxWithSource {
public:
    virtual char readChar() = 0;

    int readInt() {
        int result = 0;
        for (char c = readChar(); !this->eof() && c >= '0' && c <= '9'; c = readChar()) {
            result = 10 * result + c - '0';
        }
        return result;
    }

    double readDouble() {
        double result = 0;
        char c;
        for (c = readChar(); !this->eof() && c >= '0' && c <= '9'; c = readChar()) {
            result = 10 * result + c - '0';
        }
        if (!this->eof()) {
            if (c == '.') {
                c = readChar();
                for (int i = 0; !this->eof() && c >= '0' && c <= '9'; i++) {
                    result += static_cast<double>(c - '0') / static_cast<double>(pow(10, i));
                    c = readChar();
                }
            }
        }
        return result;
    }

    std::string readString(char stopAt = '\n') {
        std::string str;
        for (char c = readChar(); c != stopAt && !this->eof(); c = readChar()) {
            str.append(&c);
        }
        return str;
    }
};

class Writer: BoxWithSource {
public:
    virtual void write(char val) = 0;

    void write(int val) {
        std::vector<int> vec;
        while (val != 0) {
            vec.push_back(val % 10);
            val /= 10;
        }
        for (int i = 0; i < vec.size(); i++) {
            write(static_cast<char>(vec[i]));
        }
    }

    void write(double val) {
        int intager = static_cast<int>(val);
        int fractional = static_cast<int>(8 * (val - intager));
        write(intager);
        write('.');
        write(fractional);
    }

    void write(std::string str) {
        for (int i = 0; i < str.size(); i++) {
            write(str[i]);
        }
    }
};

class ReaderWriter: Reader, Writer {};

class StringReaderWriter: ReaderWriter {
private:
    std::string source;
    size_t idx;

public:
    StringReaderWriter(std::string str): source(str), idx(0) {};

    bool eof() {
        return idx >= source.length();
    }

    bool isOpen() {
        return idx < source.length();
    }

    virtual void close() {
        idx = source.length();
    }

    virtual char readChar() {
        return source[idx++];
    }

    virtual void write(char val) {
        source.append(&val);
    }
};

class FileReaderWriter: ReaderWriter {
private:
    FILE* source;
    bool open;

public:
    FileReaderWriter(char* fileName): source(fopen(fileName, "a+")), open(true) {};

    FileReaderWriter(std::string fileName): source(fopen(fileName.c_str(), "a+")), open(true) {};

    bool eof() {
        return feof(source);
    }

    bool isOpen() {
        return open;
    }

    virtual void close() {
        open = false;
        fclose(source);
    }

    virtual char readChar() {
        return fgetc(source);
    }

    virtual void write(char val) {
        fprintf(source, "%c", val);
    }
};


class BufferStringRW: StringReaderWriter {
private:
    char* buffer;
    int size;
    int widx;
    int ridx;

public:
    BufferStringRW(std::string str, int bufferSize): StringReaderWriter(str), buffer(new char[bufferSize]), size(bufferSize), widx(0), ridx(0) {};

    void close() {
        for (int i = 0; i < widx; i++){
            StringReaderWriter::write(buffer[i]);
        }
        StringReaderWriter::close();
    }

    char readChar() {
        char val;
        if (ridx >= widx) {
            val = buffer[ridx++];
        } else {
            val = StringReaderWriter::readChar();
        }
        return val;
    }

    void write(char val) {
        if (widx >= size) {
            buffer[widx++] = val;
        } else {
            StringReaderWriter::write(val);
        }
    }
};


class BufferFileRW: FileReaderWriter {
private:
    char* buffer;
    int size;
    int widx;
    int ridx;

public:
    BufferFileRW(char* fileName, int bufferSize): FileReaderWriter(fileName), buffer(new char[bufferSize]), size(bufferSize), widx(0), ridx(0) {};

    BufferFileRW(std::string fileName, int bufferSize): FileReaderWriter(fileName), buffer(new char[bufferSize]), size(bufferSize), widx(0), ridx(0) {};

    void close() {
        for (int i = 0; i < widx; i++){
            FileReaderWriter::write(buffer[i]);
        }
        FileReaderWriter::close();
    }

    char readChar() {
        char val;
        if (ridx >= widx) {
            val = buffer[ridx++];
        } else {
            val = FileReaderWriter::readChar();
        }
        return val;
    }

    void write(char val) {
        if (widx >= size) {
            buffer[widx++] = val;
        } else {
            FileReaderWriter::write(val);
        }
    }
};

int main() {}
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include <cassert>


class BoxWithSource {
public:
    virtual bool eof() = 0;
    virtual bool isOpen() = 0;
    virtual void close() = 0;
};

class Reader: BoxWithSource {
public:
    virtual char readChar() = 0;

    int readInt() {
        int result = 0;
        for (char c = readChar(); !eof() && c >= '0' && c <= '9'; c = readChar()) {
            result = 10 * result + c - '0';
        }
        return result;
    }

    double readDouble() {
        double result = readInt();
        if (!eof()) {
            double frac = readInt();
            result += frac/std::pow(10, static_cast<int>(std::log10(frac)) + 1);
        }
        return result;
    }

    std::string readString(char stopAt = '\n') {
        std::string str{};
        for (char c = readChar(); c != stopAt && !eof(); c = readChar()) {
            str.push_back(c);
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
        for (int i = vec.size() - 1; i >= 0; i--) {
            write(static_cast<char>(vec[i] + '0'));
        }
    }

    void write(double val) {
        int intager = static_cast<int>(val);
        std::stringstream ss;
        ss << (val - intager);
        int fractional = static_cast<int>(std::round(std::pow(10, ss.str().length() - 2) * (val - intager)));
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

class ReaderWriter: public Reader, public Writer {};

class StringReaderWriter: public ReaderWriter {
private:
    std::string source;
    size_t idx;

public:
    using Writer::write;

    StringReaderWriter(std::string str): source(str), idx(0) {};

    bool eof() {
        return idx > source.length();
    }

    bool isOpen() {
        return idx < source.length();
    }

    virtual void close() {
        //idx = source.length();
    }

    virtual char readChar() {
        return source[idx++];
    }

    virtual void write(char val) {
        source.push_back(val);
    }
};

class FileReaderWriter: public ReaderWriter {
private:
    FILE* source;
    size_t idx;
    bool open;

public:
    using Writer::write;

    FileReaderWriter(char* fileName): source(fopen(fileName, "a+")), open(true), idx(0) {};

    FileReaderWriter(std::string fileName): source(fopen(fileName.c_str(), "a+")), open(true), idx(0) {};

    ~FileReaderWriter() {
        close();
    }

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
        fseek(source, idx++, SEEK_SET);
        return fgetc(source);
    }

    virtual void write(char val) {
        fprintf(source, "%c", val);
        fflush(source);
    }
};


class BufferStringRW: public StringReaderWriter {
private:
    char* buffer;
    int size;
    int widx;
    int ridx;

public:
    using Writer::write;

    BufferStringRW(std::string str, int bufferSize): StringReaderWriter(str), buffer(new char[bufferSize]), size(bufferSize), widx(0), ridx(0) {};

    void close() {
        for (int i = 0; i < widx; i++){
            StringReaderWriter::write(buffer[i]);
        }
        StringReaderWriter::close();
    }

    char readChar() {
        char val;
        //std::cout << "{" << ridx << '-' << buffer[ridx] << '-' << widx << "}" << std::endl;
        if (ridx < widx) {
            val = buffer[ridx++];
        } else {
            val = StringReaderWriter::readChar();
        }
        return val;
    }

    void write(char val) {
        if (widx < size) {
            buffer[widx++] = val;
        } else {
            StringReaderWriter::write(val);
        }
    }
};


class BufferFileRW: public FileReaderWriter {
private:
    char* buffer;
    int size;
    int widx;
    int ridx;

public:
    using Writer::write;

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
        if (ridx < widx) {
            val = buffer[ridx++];
        } else {
            val = FileReaderWriter::readChar();
        }
        return val;
    }

    void write(char val) {
        if (widx < size) {
            buffer[widx++] = val;
        } else {
            FileReaderWriter::write(val);
        }
    }
};

void test1() {
    StringReaderWriter srw(std::string("c123 11.20 string")); 
    assert(srw.readChar() == 'c');
    assert(srw.readInt() == 123);
    assert(srw.readDouble() == 11.2);
    assert(srw.readString() == std::string("string"));
}

void test2() {
    StringReaderWriter srw(std::string{});
    srw.write(321);
    srw.write(' ');
    srw.write(2.11);
    srw.write(std::string("gnirts"));
    assert(srw.readString() == std::string("321 2.11gnirts"));
}

void test3() {
    FILE* file = fopen("file.txt", "w");
    fprintf(file, "1 2.3 4 56 7");
    fclose(file);
    FileReaderWriter frw(std::string("file.txt"));
    assert(frw.readInt() == 1);
    assert(frw.readDouble() == 2.3);
    assert(frw.readChar() == '4');
    assert(frw.readString() == std::string(" 56 7"));
}

void test4() {
    FILE* file = fopen("file.txt", "w");
    fclose(file);
    FileReaderWriter frw(std::string("file.txt"));
    frw.write(3.3);
    frw.write('=');
    frw.write(3);
    frw.write(std::string(" + 0.30"));
    assert(frw.readInt() == 3);
    assert(frw.readString(' ') == std::string("3=3"));
    assert(frw.readChar() == '+');
    assert(frw.readChar() == ' ');
    assert(frw.readDouble() == 0.30);
}

void test5() {
    std::string str{"0.33 string1"};
    BufferStringRW bsrw(str, 5);
    bsrw.write(321);
    bsrw.write(' ');
    bsrw.write('@');
    assert(bsrw.readInt() == 321);
    //std::cout << "<" << bsrw.readChar() << ">" << std::endl;
    assert(bsrw.readChar() == '@');
    assert(bsrw.readDouble() == 0.33);
    assert(bsrw.readString() == std::string("string1"));
    bsrw.write(' ');
    bsrw.write(1.88);
    bsrw.write(std::string("string2"));
    bsrw.close();
    //std::cout << bsrw.readString() << std::endl;
    assert(bsrw.readString() == std::string("1.88string2321 @"));
}

void test6() {
    FILE* file1 = fopen("file.txt", "w");
    fprintf(file1, "33/10 =");
    fclose(file1);

    BufferFileRW bfrw(std::string("file.txt"), 6);
    bfrw.write(3.3);
    bfrw.write(' ');
    assert(bfrw.readDouble() == 3.3);
    assert(bfrw.readString() == std::string("33/10 ="));
    bfrw.write(0);
    bfrw.write(std::string("   "));
    assert(bfrw.readInt() == 0);
    assert(bfrw.readChar() == ' ');
    bfrw.close();

    FILE* file2 = fopen("file.txt", "r");
    char cstr[15];
    fgets(cstr, 15, file2);
    fclose(file2);
    assert(std::string(cstr) == std::string("33/10 = 3.3   "));
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
}
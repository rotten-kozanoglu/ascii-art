#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string ascii_chars = "    @#MW&%B8Q$O0XZoc>!^/|()=~:;,.'\"`    ";

char to_ascii(int val, int max_val) {
    int idx = (int)((val / (double)max_val) * ascii_chars.size());
    return ascii_chars[idx];
}

int main() {
    string filename;
    cout << "Enter image filename: ";
    cin >> filename;

    ifstream infile(filename.c_str(), ios::binary);
    if (!infile) {
        cerr << "Error opening file " << filename << endl;
        return 1;
    }

    char c1, c2;
    infile.read(&c1, 1);
    infile.read(&c2, 1);
    if (c1 != 'B' || c2 != 'M') {
        cerr << "Not a BMP file." << endl;
        return 1;
    }

    infile.seekg(10);
    int offset;
    infile.read((char*)&offset, 4);

    infile.seekg(18);
    int width, height;
    infile.read((char*)&width, 4);
    infile.read((char*)&height, 4);

    infile.seekg(offset);

    unsigned char* data = new unsigned char[width * height * 3];
    infile.read((char*)data, width * height * 3);

    ofstream outfile("output.txt");
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 3;
            int val = (int)(0.2126 * data[idx] + 0.7152 * data[idx + 1] + 0.0722 * data[idx + 2]);
            outfile << to_ascii(val, 255);
        }
        outfile << endl;
    }
    outfile.close();

    delete[] data;
    return 0;
}
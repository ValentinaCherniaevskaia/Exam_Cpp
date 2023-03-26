/*
 *    ��������� ����������� �� C++
 *
 *    ������ 2020
 */
#include <iostream>
#include <stdexcept>

using namespace std; //������������ ���

const char* err_index = "index out of range";
const char* err_dim = "illegal dimension";


class Matrix
{
private:          //���������� ����������� � �������� �������
    int** mx;      //��������� �� ������ ����� ���������� �������
    size_t rows;     //���-�� �����
    size_t cols;     //���-�� ��������

    void create(size_t _rows, size_t _cols) {
        rows = _rows;
        cols = _cols;
        mx = new int* [rows];
        for (size_t i = 0; i < rows; i++)
            mx[i] = nullptr;
        for (size_t i = 0; i < rows; i++) //���� �� ���������� ���������� �����
        {
            mx[i] = new int[cols]; //^� ��������
        }
    }

    void destroy() {
        if (mx != nullptr) {
            for (size_t i = 0; i < rows; i++)
                if (mx[i] != nullptr)
                    delete[] mx[i];
            delete[] mx;
        }
    }

    void copy(const Matrix& op) {
        create(op.cols, op.rows);

        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                mx[i][j] = op.mx[i][j];
    }

    void move(Matrix& op) noexcept {
        cols = op.cols;
        rows = op.rows;
        mx = op.mx;
        op.cols = op.rows = 0;
        op.mx = nullptr;
    }

public:
    // �����������
    Matrix() : mx(nullptr), rows(0), cols(0)
    {
        std::cout << "Enter the number of lines: "; //������� ���������� �����
        std::cin >> rows; //����� "n" �� �����
        std::cout << "Enter the number of colums: "; //������� ���������� ��������
        std::cin >> cols;//����� "m" �� �����

        create(rows, cols);
    }

    // ����������� � ����� �����������
    Matrix(size_t n, size_t m) : mx(nullptr), rows(0), cols(0)
    {
        create(n, m);
    }

    // ����������� �����
    Matrix(const Matrix& op) : mx(nullptr), rows(0), cols(0) {
        copy(op);
    }

    // ����������� ����� � ������������
    Matrix(Matrix&& op) noexcept : mx(nullptr), rows(0), cols(0) {
        move(op);
    }

    // ����������: ���������� ������� � ������������ ������
    ~Matrix()
    {
        destroy();
    }

    // ������������� �������� ������������
    Matrix& operator = (const Matrix& op) {
        destroy();
        copy(op);
        return *this;
    }

    // ������������� �������� ������������ � ������������
    Matrix& operator = (Matrix&& op) noexcept {
        destroy();
        move(op);
        return *this;
    }

    // ���� �������
    void SetMatrix()
    {
        std::cout << "Enter matrix item: " << this << endl; //������� ������� �������
        for (size_t i = 0; i < rows; i++) //���� �� ����
        {
            for (size_t j = 0; j < cols; j++)
            {
                std::cout << "[" << i << "][" << j << "]="; //����� "i" � "j"
                std::cin >> mx[i][j]; //���� ��� � ������ "A"
            }
        }
    }

    //����� �������
    void PrintMatrix(const char* header) const
    {
        std::cout << "\nOutput " << header << "\n\n";


        for (size_t i = 0; i < rows; i++) //���� ��� ����������� ������
        {
            for (size_t j = 0; j < cols; j++)
            {
                std::cout << mx[i][j] << "      ";
            }
            std::cout << endl;
        }
    }

    int Get_Element(size_t r, size_t c) const
    {
        if (r >= rows || c >= cols)
            throw out_of_range(err_index);
        return mx[r][c];
    }

    // �������� ������� � ��������
    int& operator () (size_t r, size_t c) {
        if (r >= rows || c >= cols)
            throw out_of_range(err_index);
        return mx[r][c];
    }

    // ���������� ���������� �����
    size_t Get_Row_Matrix() const
    {
        return rows;
    }

    // ���������� ���������� ��������
    size_t Get_Column_Matrix() const
    {
        return cols;
    }

    // ���������� ����������������� �������
    Matrix Transpose() const
    {
        Matrix S(cols, rows);
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                S.mx[j][i] = mx[i][j];
        return S;
    }

    friend Matrix operator + (const Matrix& A, const Matrix& B)
    {
        if (A.rows != B.rows || A.cols != B.cols)
            throw invalid_argument(err_dim);

        Matrix S(A.rows, A.cols);
        for (size_t i = 0; i < A.rows; i++)
        {
            for (size_t j = 0; j < A.cols; j++)
            {
                S.mx[i][j] = A.mx[i][j] + B.mx[i][j];
            }
        }
        return S;
    }

    friend Matrix operator - (const Matrix& A, const Matrix& B)
    {
        if (A.rows != B.rows || A.cols != B.cols)
            throw invalid_argument(err_dim);

        Matrix S(A.rows, A.cols);
        for (size_t i = 0; i < A.rows; i++)
        {
            for (size_t j = 0; j < A.cols; j++)
            {
                S.mx[i][j] = A.mx[i][j] - B.mx[i][j];
            }
        }
        return S;
    }

    friend Matrix operator * (const Matrix& A, const Matrix& B)
    {
        if (A.rows != B.cols || A.cols != B.rows)
            throw invalid_argument(err_dim);

        Matrix S = Matrix(A.rows, B.cols);

        for (size_t i = 0; i < S.rows; i++)
        {
            for (size_t j = 0; j < S.cols; j++)
            {
                S.mx[i][j] = 0;
                for (size_t k = 0; k < A.cols; k++)
                    S.mx[i][j] += A.mx[i][k] * B.mx[k][j];
            }
        }
        return S;
    }

};

int main()
{
    try {
        Matrix mx1; // ���������� ��������� ������
        mx1.SetMatrix(); //���������� � ������ ������� (����� �����)
        mx1.PrintMatrix("matrix 1");
        mx1 = mx1.Transpose();
        mx1.PrintMatrix("transposed matrix 1");

        Matrix mx2;
        mx2.SetMatrix();
        mx2.PrintMatrix("matrix 2"); //������� �������
        mx2 = mx2.Transpose();
        mx2.PrintMatrix("transposed matrix 2");

        std::cout << "-------------------------------------" << endl;

        (mx1 + mx2).PrintMatrix("result sum matrix");

        (mx1 - mx2).PrintMatrix("result substraction matrix");

        Matrix mx3 = mx1 + mx2;
        mx3.PrintMatrix("matrix 3 is sum of matrix 1 and 2");

        mx2 = mx2.Transpose();
        (mx1 * mx2).PrintMatrix("result muitiplication matrix");

        for (size_t i = 0; i < mx1.Get_Row_Matrix(); ++i)
            for (size_t j = 0; j < mx1.Get_Column_Matrix(); ++j)
                if (mx1(i, j) % 2)
                    mx1(i, j) = 0;
        mx1.PrintMatrix("direct access to matrix elements");
    }
    catch (exception& e) {
        std::cout << "exception: " << e.what() << endl;
    }

    return 0;
}
#include <iostream>

class Matrix {
  class ProxyRow {
   private:
   public:
    ProxyRow(size_t ncols) {
      ncols_ = ncols;
      data_ = new int32_t[ncols_];
      for (size_t i = 0; i < ncols_; i++) data_[i] = 0;
    };
    ~ProxyRow() { delete[] data_; };
    int32_t& operator[](size_t j) {
      if (j >= ncols_) throw std::out_of_range("[]");
      return data_[j];
    }
    ProxyRow operator+(const ProxyRow& row) const {
      if (ncols_ != row.ncols_) throw std::invalid_argument("+");
      ProxyRow row_return(ncols_);
      for (size_t i = 0; i < ncols_; i++)
        row_return.data_[i] = data_[i] + row.data_[i];
      return row_return;
    };

    ProxyRow operator*=(int32_t value) {
      for (size_t i = 0; i < ncols_; i++) data_[i] *= value;
      return *this;
    };

    bool operator==(const ProxyRow& row) const {
      if (ncols_ != row.ncols_) return false;
      for (size_t i = 0; i < ncols_; i++)
        if (data_[i] != row.data_[i]) return false;
      return true;
    };

    friend std::ostream& operator<<(std::ostream& out, const ProxyRow& row) {
      for (size_t i = 0; i < row.ncols_; i++) out << row.data_[i] << ' ';
      return out;
    };

    size_t ncols_;
    int32_t* data_;
  };

 public:
  Matrix(size_t nrows, size_t ncols) {
    nrows_ = nrows;
    ncols_ = ncols;

    rows_ = new ProxyRow*[nrows];
    for (size_t i = 0; i < nrows_; i++) {
      rows_[i] = new ProxyRow(ncols_);
    }
  };
  ~Matrix() {
    for (size_t i = 0; i < nrows_; i++) delete rows_[i];
    delete[] rows_;
  }
  size_t getRows() const { return nrows_; };
  size_t getColumns() const { return ncols_; };

  ProxyRow& operator[](size_t i) {
    if (i >= nrows_) throw std::out_of_range("[]");
    return *(rows_[i]);
  };

  Matrix operator+(const Matrix& matrix) const {
    if (nrows_ != matrix.nrows_) throw std::invalid_argument("+");
    Matrix matrix_return(nrows_, ncols_);
    for (size_t i = 0; i < nrows_; i++) {
      *(matrix_return.rows_[i]) = *(rows_[i]) + *(matrix.rows_[i]);
    }
    return matrix_return;
  }

  Matrix operator*=(int32_t value) {
    for (size_t i = 0; i < nrows_; i++) *(rows_[i]) *= value;
    return *this;
  }

  bool operator==(const Matrix& matrix) const {
    if (nrows_ != matrix.nrows_) return false;
    for (size_t i = 0; i < nrows_; i++)
      if (*(rows_[i]) == *(matrix.rows_[i]))
        continue;
      else
        return false;
    return true;
  }

  bool operator!=(const Matrix& matrix) const { return not(*this == matrix); }

  friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
    for (size_t i = 0; i < matrix.nrows_; i++)
      out << *(matrix.rows_[i]) << std::endl;
    return out;
  }

  ProxyRow** rows_;
  size_t nrows_;
  size_t ncols_;
};

#include <algorithm>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using num_type = std::vector<uint64_t>;

class Sorter {
 public:
  std::string infile;
  size_t filelen;
  std::string outfile;

  std::vector<std::string>
      blocknames;
  std::vector<size_t> used_bounds;

  std::mutex file;
  std::mutex bounds;
  std::mutex mutBlocknames;

  size_t step;

  bool mistake = false;

  Sorter() {}
  ~Sorter() {}

  bool getNumbers(num_type &vect, size_t a, size_t b) {
    std::ifstream stream;
    uint64_t number;
    stream.open(infile, std::ios::binary);

    if (!stream) return false;

    for (size_t i = 0; i < a + b; i++) {
      stream.read((char *)&number, sizeof(uint64_t));
      if (i >= a) vect.push_back(number);
    }
    stream.close();
    return true;
  };

  void getChanges(bool &ready, size_t &thr_curr, size_t &thr_step) {
    while (true) {
      std::vector<size_t>::iterator it =
          std::find(used_bounds.begin(), used_bounds.end(), thr_curr);
      if (it != used_bounds.end()) {
        if (thr_curr + thr_step < filelen)
          thr_curr += thr_step;
        else {
          ready = true;
          break;
        }
      } else {
        if (thr_curr + thr_step > filelen) thr_step = filelen - thr_curr;

        used_bounds.push_back(thr_curr);
        break;
      }
    }
  };
  void sortBlocks() {
    size_t thr_curr = 0;
    size_t thr_step = step;

    bool ready = false;

    bounds.lock();
    getChanges(ready, thr_curr, thr_step);
    bounds.unlock();

    num_type vect;

    std::string block = "block";
    std::string bin = ".bin";
    std::string blockname;

    std::ofstream oStream;

    while (!ready) {
      vect.clear();
      file.lock();
      if (!getNumbers(vect, thr_curr, thr_step)) {
        mistake = true;
        break;
      }
      file.unlock();

      std::sort(vect.begin(), vect.end());

      block = "block";
      blockname = block.append(std::to_string(thr_curr)).append(bin);

      mutBlocknames.lock();
      blocknames.push_back(blockname);
      mutBlocknames.unlock();

      oStream.open(blockname, std::ios::binary);
      if (!oStream) {
        mistake = true;
        break;
      }

      for (size_t i = 0; i < thr_step; i++)
        oStream.write((char *)&vect[i], sizeof(uint64_t));

      oStream.close();

      bounds.lock();
      getChanges(ready, thr_curr, thr_step);
      bounds.unlock();
    }
  };
  void merge() {
    if (mistake) {
      for (size_t j = 0; j < blocknames.size(); j++)
        remove(blocknames[j].c_str());
    }

    uint64_t number;
    std::string str;

    std::ofstream result;
    result.open(outfile, std::ios::binary);

    if (!result) {
      mistake = true;

      for (size_t j = 0; j < blocknames.size(); j++)
        remove(blocknames[j].c_str());

      return;
    }

    size_t nBlocks = blocknames.size();
    std::vector<std::ifstream> blockstreams(nBlocks);
    num_type numbers;

    std::vector<uint64_t>::iterator minIter;
    size_t minIdx;

    for (size_t i = 0; i < nBlocks; i++) {
      blockstreams[i].open(blocknames[i], std::ios::binary);
      if (!blockstreams[i]) {
        mistake = true;
        for (size_t j = 0; j < i - 1; j++) blockstreams[j].close();

        for (size_t j = 0; j < blocknames.size(); j++)
          remove(blocknames[j].c_str());

        return;
      }
      blockstreams[i].read((char *)&number, sizeof(uint64_t));
      numbers.push_back(number);
    }

    while (true) {
      minIter = std::min_element(numbers.begin(), numbers.end());
      minIdx = std::abs(std::distance(numbers.begin(), minIter));
      result.write((char *)&numbers[minIdx], sizeof(uint64_t));

      if (!blockstreams[minIdx].read((char *)&number, sizeof(uint64_t))) {
        blockstreams[minIdx].close();
        remove(blocknames[minIdx].c_str());
        blockstreams.erase(blockstreams.begin() + minIdx);
        blocknames.erase(blocknames.begin() + minIdx);
        numbers.erase(numbers.begin() + minIdx);
        nBlocks -= 1;
        if (nBlocks == 0) break;

      } else
        numbers[minIdx] = number;
    }
    result.close();
  };
  bool sort(const std::string &name, const std::string &res, size_t len,
            size_t st) {
    infile = name;
    outfile = res;
    filelen = len;
    step = st;
    mistake = false;

    blocknames.clear();
    used_bounds.clear();

    std::thread firstSort(&Sorter::sortBlocks, this);
    std::thread secondSort(&Sorter::sortBlocks, this);
    firstSort.join();
    secondSort.join();
    merge();
    return !mistake;
  };
};
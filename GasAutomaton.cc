//
// Created by henry on 3/20/18.
//

#include "GasAutomaton.hpp"

//#define NO_WALLS
#define NO_FREE_PARTICLES

inline cv::Vec3b rgb(Automata::dataType r, Automata::dataType g, Automata::dataType b) {
    return {uchar(b), uchar(g), uchar(r)};
}
Automata::GasAutomaton::GasAutomaton(size_t totalSteps, std::string outputPath) :
    CellularAutomaton(totalSteps), outputPath(std::move(outputPath)) {
//  this->frame = cv::Mat::zeros(GasAutomaton::size, GasAutomaton::size, CV_8UC3);
    this->init();
}

void Automata::GasAutomaton::init() noexcept {
    if (this->writer.isOpened()) {
        this->writer.release();
    }
    this->writer
        .open(this->outputPath,
              CV_FOURCC('M', 'P', 'E', 'G'),
              30,
              cv::Size(GasAutomaton::size, GasAutomaton::size),
              true
        );
    if (!this->writer.isOpened()) {
        this->_hasErrorOcurred = true;
        std::cout << "Cannot open file" << std::endl;
        return;
    }

    // Random
    std::random_device rd {};
    std::mt19937 gen {rd()};
    std::uniform_int_distribution id(0, 4);
    std::normal_distribution<> d {5, 2};
    double a;

    for (unsigned i = 0; i < GasAutomaton::size; ++i) {
        for (unsigned j = 0; j < GasAutomaton::size; ++j) {
            if ((j > (GasAutomaton::size >> 1U)) && (a = d(gen), a > 8 || a < 2)) {
                _nextLattice(i, j) = (1 << (id(gen)));
            } else {
                _nextLattice(i, j) = 0;
            }
            set<Solid>(i, j, false);
        }
        set<Solid>(i, 0);
        set<Solid>(i, GasAutomaton::size - 1);
        set<Solid>(0, i);
        set<Solid>(GasAutomaton::size - 1, i);
    }

    static constexpr std::size_t nthPartOfSize = GasAutomaton::size / 7;
    for (unsigned row = 0; row < GasAutomaton::size; ++row) {
        if (row < (3 * nthPartOfSize) || row >= 4 * nthPartOfSize) {
            _nextLattice(row, 200) = 0U;
            set<Solid>(row, 200);
        }
    }
    this->_currentLattice.swap(this->_nextLattice);
    // First second of the video is the starting condition
    for (int i = 0; i < 30; ++i) {
        draw();
    }

}

void Automata::GasAutomaton::applyRule() {
    for (unsigned x = 0; x < GasAutomaton::size; ++x) {
        size_t wneigh = x == 0 ? (GasAutomaton::size - 1) : x - 1, eneigh = x == GasAutomaton::size - 1 ? 0 : x + 1;
        for (unsigned y = 0; y < GasAutomaton::size; ++y) {
            size_t nneigh = y == 0 ? GasAutomaton::size - 1 : y - 1, sneigh = y == GasAutomaton::size - 1 ? 0 : y + 1;
            bool
                n = get<North>(x, y),
                s = get<South>(x, y),
                e = get<East>(x, y),
                w = get<West>(x, y),
                nw = get<Northwest>(x, y),
                ne = get<Northeast>(x, y),
                sw = get<Southwest>(x, y),
                se = get<Southeast>(x, y),
                sol = get<Solid>(x, y);

            // TODO: Finish this...

            // n && !s && !sol || !n && s && sol || !n && !s && e && w
            set<North>(x, nneigh, (n && !s && !sol || !n && s && sol || !n && !s && e && w));
            // s && !n && !sol || !s && n && sol || !n && !s && e && w
            set<South>(x, sneigh, (s && !n && !sol || !s && n && sol || !n && !s && e && w));
            // e && !w && !sol || !e && w && sol || !e && !w && n && s
            set<East>(eneigh, y, (e && !w && !sol || !e && w && sol || !e && !w && n && s));
            // w && !e && !sol || !w && e && sol || !w && !e && n && s
            set<West>(wneigh, y, (w && !e && !sol || !w && e && sol || !w && !e && n && s));
            set<Solid>(x, y, sol);
        }
    }
    this->_currentStep++;
    this->_currentLattice.swap(this->_nextLattice);
    draw();
}

void Automata::GasAutomaton::draw() {
    this->frame = cv::Mat::zeros(GasAutomaton::size, GasAutomaton::size, CV_8UC3);
    for (unsigned row = 0; row < GasAutomaton::size; ++row) {
        for (unsigned col = 0; col < GasAutomaton::size; ++col) {
            if (get<Solid>(col, row)) {
                frame.at<cv::Vec3b>(col, row) = rgb(255, 0, 0);
            } else if (_currentLattice(col, row) > 0) {
                if (get<North>(col, row)) {
                    frame.at<cv::Vec3b>(col, row) = rgb(0, 255U, 0);
                } else if (get<South>(col, row)) {
                    frame.at<cv::Vec3b>(col, row) = rgb(255U, 0, 0);
                } else if (get<East>(col, row)) {
                    frame.at<cv::Vec3b>(col, row) = rgb(100, 100U, 0);
                } else {
                    frame.at<cv::Vec3b>(col, row) = rgb(0, 100U, 100U);
                }
            } else {
                frame.at<cv::Vec3b>(col, row) = rgb(0U, 0U, 0U);
            }
        }
    }

    this->writer.write(this->frame);
    std::cout << std::setw(5) << this->_currentStep << "/" << std::setw(5) << this->_totalSteps << "\r" << std::flush;
}
Automata::GasAutomaton::~GasAutomaton() noexcept {
    this->writer.release();
}

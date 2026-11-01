#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "ArtDigits.hpp"
#include "utils.hpp"

inline constexpr auto MSPerTick = 50;

namespace detail {

template <std::size_t Height, std::size_t Width>
static void printBlock(const char (&object)[Height][Width], int baseRow,
                       int baseCol) {
  for (int row = 0; row != Height; ++row) {
    move_cursor(baseRow + row, baseCol);
    std::cout << object[row];
  }
  std::cout << std::flush;
}

} // namespace detail

class DigitPad {
    TextColor mColor;
    int mBaseRow;
    int mBaseCol;
    int mCurrentDigit;

public:
    enum class ScrollDirection { Up, Down };

    DigitPad(TextColor color, int baseRow, int baseCol)
        : mColor{color}, mBaseRow{baseRow}, mBaseCol{baseCol}, mCurrentDigit{0} {}

    int getBaseRow() const { return mBaseRow; }
    int getBaseCol() const { return mBaseCol; }
    int getDigit() const { return mCurrentDigit; }

    void setColor(TextColor color) {
        mColor = color;
    }

    void print() const {
        ColorGuard colorGuard(mColor);
        detail::printBlock(ArtDigit[mCurrentDigit], mBaseRow, mBaseCol);
    }

    void updateAndScroll(int newDigit, ScrollDirection scrollDir) {
        int oldDigit = mCurrentDigit;
        mCurrentDigit = newDigit;
        int topDigit = scrollDir == ScrollDirection::Up ? oldDigit : newDigit;
        int bottomDigit = oldDigit + newDigit - topDigit;
        std::cout << mColor;
        for (int tick = 0; tick != DigitHeight + 1; ++tick) {
            int mid = scrollDir == ScrollDirection::Up ? tick : DigitHeight - tick;
            int row = mBaseRow;
            for (int i = mid; i != DigitHeight; ++i) {
                move_cursor(row++, mBaseCol);
                std::cout << ArtDigit[topDigit][i];
            }
            for (int i = 0; i != mid; ++i) {
                move_cursor(row++, mBaseCol);
                std::cout << ArtDigit[bottomDigit][i];
            }
            std::cout << std::flush;
            sleep_ms(MSPerTick);
        }
        std::cout << TextColor::Normal;
    }
};

class Player {
    std::string mName;
    DigitPad mTensPlace;
    DigitPad mOnesPlace;
    DigitPad mOverAll;
    int mScore;
    int mOverAllScore;

public:
    enum class Side { Left, Right };

    Player(std::string name, Side side)
        : mName{std::move(name)},
          mTensPlace{ColorOfSide(side), DigitBaseRow, BaseColOfSide(side)},
          mOnesPlace{ColorOfSide(side), DigitBaseRow, BaseColOfSide(side) + FrameWidth},
          mOverAll{TextColor::Normal, DigitBaseRow, BaseColOfOverAll(side)}, // Overall score is white
          mScore{0},
          mOverAllScore{0} {
        if (side == Side::Left)
            clear_screen();
        move_cursor(FrameHeight, BaseColOfSide(side) - DigitBaseCol + FrameWidth - mName.size() / 2);
        std::cout << mName << std::flush;
    }

    int getScore() const { return mScore; }

    int getOverAllScore() const { return mOverAllScore; }

    void print() const {
        mTensPlace.print();
        mOnesPlace.print();
        mOverAll.print();
    }

    void updateScore(int delta) {
        int newScore = mScore + delta;
        if (newScore < 0 || newScore >= 100) {
            throw std::out_of_range("Score out of range");
        }

        PrinterGuard printerGuard;

        auto dir = delta > 0 ? DigitPad::ScrollDirection::Up : DigitPad::ScrollDirection::Down;
        mOnesPlace.updateAndScroll(newScore % 10, dir);
        if (mScore / 10 != newScore / 10) {
            mTensPlace.updateAndScroll(newScore / 10, dir);
        }

        mScore = newScore;
    }

    void updateOverAllScore(int delta) {
        int newScore = mOverAllScore + delta;
        if (newScore < 0 || newScore >= 10) {
            throw std::out_of_range("Score out of range");
        }

        PrinterGuard printerGuard;

        auto dir = delta > 0 ? DigitPad::ScrollDirection::Up : DigitPad::ScrollDirection::Down;
        mOverAll.updateAndScroll(newScore, dir);

        mOverAllScore = newScore;
    }

private:
    static TextColor ColorOfSide(Side side) {
        return side == Side::Left ? TextColor::Red : TextColor::Blue;
    }

    static int BaseColOfSide(Side side) {
        return side == Side::Left ? DigitBaseCol : (FrameWidth * 4 + ColonWidth + DigitBaseCol);
    }

    static int BaseColOfOverAll(Side side) {
        return side == Side::Left ? DigitBaseCol + FrameWidth * 2 : DigitBaseCol + FrameWidth * 3 + ColonWidth;
    }
};

class ScoreBoard {
    Player mLeft;
    Player mRight;

public:
    ScoreBoard(std::string leftName, std::string rightName)
        : mLeft{std::move(leftName), Player::Side::Left},
          mRight{std::move(rightName), Player::Side::Right} {
        printFrame();
        mLeft.print();
        mRight.print();
    }
    void printFrame() const {
        std::cout << TextColor::Red;
        detail::printBlock(Frame, 0, 0);
        detail::printBlock(Frame, 0, FrameWidth);

        std::cout << TextColor::Blue;
        detail::printBlock(Frame, 0, FrameWidth * 4 + ColonWidth);
        detail::printBlock(Frame, 0, FrameWidth * 5 + ColonWidth);

        std::cout << TextColor::Normal;
        detail::printBlock(Colon, 0, FrameWidth * 3);
    }
    void leftInc(int delta = 1) { mLeft.updateScore(delta); }
    void rightInc(int delta = 1) { mRight.updateScore(delta); }
    void leftDec(int delta = 1) { mLeft.updateScore(-delta); }
    void rightDec(int delta = 1) { mRight.updateScore(-delta); }
    void rightToZero() { mRight.updateScore(-mRight.getScore()); }
    void leftToZero() { mLeft.updateScore(-mLeft.getScore()); }
    void rightOverAllInc(int delta = 1) { mRight.updateOverAllScore(delta); }
    void leftOverAllInc(int delta = 1) { mLeft.updateOverAllScore(delta); }
    void rightOverAllDec(int delta = 1) { mRight.updateOverAllScore(-delta); }
    void leftOverAllDec(int delta = 1) { mLeft.updateOverAllScore(-delta); }

    ~ScoreBoard() {
        for (int row = FrameHeight; row >= 0; --row) {
            move_cursor(row, 0);
            clear_to_eol();
            sleep_ms(MSPerTick);
        }
    }
};

#endif // SCOREBOARD_HPP

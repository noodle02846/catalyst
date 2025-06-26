#include <engine/evaluation.hpp>

std::int16_t Evaluation::material(BoardManager& boardManager, chess::Color color) const noexcept {
    std::int16_t materialScore = 0;

    auto pawns = boardManager.getPieceCount(chess::PieceType::PAWN, color);
    auto knights = boardManager.getPieceCount(chess::PieceType::KNIGHT, color);
    auto bishops = boardManager.getPieceCount(chess::PieceType::BISHOP, color);
    auto rooks = boardManager.getPieceCount(chess::PieceType::ROOK, color);
    auto queens = boardManager.getPieceCount(chess::PieceType::QUEEN, color);

    materialScore += pawns * this->kPawnValue;
    materialScore += knights * this->kKnightValue;
    materialScore += bishops * this->kBishopValue;
    materialScore += rooks * this->kRookValue;
    materialScore += queens * this->kQueenValue;

    return materialScore;
}

std::int16_t Evaluation::pieceSquare(BoardManager& boardManager, chess::Color color) const noexcept {
    std::int16_t pieceSquareScore = 0;

    auto chessBoard = boardManager.internal();
    auto pawnsBoard = chessBoard.pieces(chess::PieceType::PAWN, color);

    for (int pawnIndex = 0; pawnIndex < pawnsBoard.count(); ++pawnIndex) {
        pieceSquareScore += this->kPawnSquareTable.at(pawnsBoard.pop());
    }

    return pieceSquareScore;
}

std::int16_t Evaluation::evaluate(BoardManager& boardManager) const noexcept {
    constexpr auto white = chess::Color::WHITE;
    constexpr auto black = chess::Color::BLACK;

    auto whiteMaterial = this->material(boardManager, white);
    auto blackMaterial = this->material(boardManager, black);

    auto whitePieceSquare = this->pieceSquare(boardManager, white);
    auto blackPieceSquare = this->pieceSquare(boardManager, black);

    std::int16_t score = 0;
    std::int16_t perspective = boardManager.turn() == white ? 1 : -1;

    score += (whiteMaterial - blackMaterial);
    score += (whitePieceSquare - blackPieceSquare);

    return score * perspective;
}
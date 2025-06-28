#include <engine/evaluation.hpp>

std::int16_t Evaluation::pieceValue(chess::PieceType pieceType) noexcept {
    switch (pieceType.internal()) {
    case chess::PieceType::NONE:
        return 0;
    case chess::PieceType::PAWN:
        return Evaluation::kPawnValue;
    case chess::PieceType::KNIGHT:
        return Evaluation::kKnightValue;
    case chess::PieceType::BISHOP:
        return Evaluation::kBishopValue;
    case chess::PieceType::ROOK:
        return Evaluation::kRookValue;
    case chess::PieceType::QUEEN:
        return Evaluation::kQueenValue;
    case chess::PieceType::KING:
        return Evaluation::kKingValue;
    }
}

std::int16_t Evaluation::material(
    BoardManager& boardManager, 
    chess::Color color
) const noexcept {
    std::int16_t materialScore = 0;

    auto pawns = boardManager.getPieceCount(this->kPawn, color);
    auto knights = boardManager.getPieceCount(this->kKnight, color);
    auto bishops = boardManager.getPieceCount(this->kBishop, color);
    auto rooks = boardManager.getPieceCount(this->kRook, color);
    auto queens = boardManager.getPieceCount(this->kQueen, color);

    materialScore += pawns * this->kPawnValue;
    materialScore += knights * this->kKnightValue;
    materialScore += bishops * this->kBishopValue;
    materialScore += rooks * this->kRookValue;
    materialScore += queens * this->kQueenValue;

    return materialScore;
}

std::int16_t Evaluation::pieceSquare(
    BoardManager& boardManager, 
    chess::Color color
) const noexcept {
    std::int16_t pieceSquareScore = 0;

    auto chessBoard = boardManager.internal();

    auto pawnBoard = chessBoard.pieces(this->kPawn, color);
    auto knightBoard = chessBoard.pieces(this->kKnight, color);
    auto bishopBoard = chessBoard.pieces(this->kBishop, color);
    auto rookBoard = chessBoard.pieces(this->kRook, color);
    auto queenBoard = chessBoard.pieces(this->kQueen, color);

    while (pawnBoard.count()) {
        pieceSquareScore += this->kPawnSquareTable[pawnBoard.pop()];
    }

    while (knightBoard.count()) {
        pieceSquareScore += this->kKnightSquareTable[knightBoard.pop()];
    }

    while (bishopBoard.count()) {
        pieceSquareScore += this->kBishopSquareTable[bishopBoard.pop()];
    }

    while (rookBoard.count()) {
        pieceSquareScore += this->kRookSquareTable[rookBoard.pop()];
    }

    while (queenBoard.count()) {
        pieceSquareScore += this->kQueenSquareTable[queenBoard.pop()];
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
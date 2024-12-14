#include "pch.h"
#include "framework.h"
#include "BrickBreaker3.h"
#include "BrickBreaker3Doc.h"
#include "BrickBreaker3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBrickBreaker3View, CView)

BEGIN_MESSAGE_MAP(CBrickBreaker3View, CView)
    ON_COMMAND(ID_GAME_START, &CBrickBreaker3View::OnStartGame) // Bắt sự kiện Start
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_WM_TIMER()
    ON_COMMAND(ID_GAME_REPLAY, &CBrickBreaker3View::OnReplay)
END_MESSAGE_MAP()

CBrickBreaker3View::CBrickBreaker3View() noexcept
    : m_ballRadius(10), m_paddleSpeed(10), m_gameRunning(false),
    m_gameStarted(false), m_moveLeft(false), m_moveRight(false), m_score(0), m_highScore(0)
{
    m_paddle = CRect(200, 400, 300, 420);
    m_ball = CPoint(250, 200);
    m_ballSpeed = CSize(5, -5);

    // Tạo các khối vuông
    int brickWidth = 56, brickHeight = 20, rows = 3, cols = 16, startX = 10, startY = 50;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            CRect brick(startX + j * (brickWidth + 5), startY + i * (brickHeight + 5),
                startX + j * (brickWidth + 5) + brickWidth,
                startY + i * (brickHeight + 5) + brickHeight);
            m_bricks.push_back(brick);
        }
    }
}

CBrickBreaker3View::~CBrickBreaker3View() {}

void CBrickBreaker3View::OnDraw(CDC* pDC)
{
    // Vẽ thanh ngang trên
    CBrush redBrush(RGB(0, 0, 0));
    CBrush* pOldBrush = pDC->SelectObject(&redBrush);
    pDC->Rectangle(0, 0, 1000, 10);  // Thanh ngang trên
    pDC->Rectangle(0, 0, 10, 430);   // Thanh trái
    pDC->Rectangle(990, 0, 1000, 430); // Thanh phải
    pDC->Rectangle(0, 420, 1000, 430); // Vẽ thanh ngang dưới
    pDC->SelectObject(pOldBrush);

    // Vẽ bóng
    CBrush blackBrush(RGB(255, 255, 255));
    pOldBrush = pDC->SelectObject(&blackBrush);
    pDC->Ellipse(m_ball.x - m_ballRadius, m_ball.y - m_ballRadius, m_ball.x + m_ballRadius, m_ball.y + m_ballRadius);
    pDC->SelectObject(pOldBrush);

    // Vẽ thanh ngang
    CBrush paddleBrush(RGB(0, 0, 0));  // Màu xanh cho thanh ngang
    pOldBrush = pDC->SelectObject(&paddleBrush);
    pDC->Rectangle(m_paddle); // Thanh ngang chính
    pDC->SelectObject(pOldBrush);

    // Vẽ các khối vuông
    CBrush brickBrush(RGB(0, 255, 0));
    pOldBrush = pDC->SelectObject(&brickBrush);
    for (const auto& brick : m_bricks)
    {
        pDC->Rectangle(brick);
    }
    pDC->SelectObject(pOldBrush);

    // Hiển thị điểm số
    CString scoreText;
    scoreText.Format(_T("Diem: %d"), m_score);
    CFont font;
    font.CreatePointFont(100, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOutW(200, m_paddle.bottom + 30, scoreText);  // Vị trí điểm số dưới thanh ngang
    pDC->SelectObject(pOldFont);

    // Hiển thị điểm số cao nhất
    CString highScoreText;
    highScoreText.Format(_T("Diem so cao nhat: %d"), m_highScore);
    pDC->TextOutW(200, m_paddle.bottom + 50, highScoreText);  // Vị trí điểm cao nhất dưới điểm số
}


void CBrickBreaker3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == 'A')
        m_moveLeft = true;
    else if (nChar == 'D')
        m_moveRight = true;

    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBrickBreaker3View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == 'A')
        m_moveLeft = false;
    else if (nChar == 'D')
        m_moveRight = false;

    CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CBrickBreaker3View::OnStartGame()
{
    // Ẩn nút Start
    m_startButton.ShowWindow(SW_HIDE);

    // Đánh dấu game đã bắt đầu
    m_gameStarted = true;
    m_gameRunning = true;
    m_score = 0; // Đặt lại điểm số về 0
    m_paddle = CRect(200, 400, 300, 420);  // Đặt lại vị trí thanh ngang
    m_ball = CPoint(250, 200);             // Đặt lại vị trí bóng
    m_ballSpeed = CSize(5, -5);            // Đặt lại tốc độ bóng

    // Đặt lại trạng thái di chuyển thanh ngang
    m_moveLeft = false;
    m_moveRight = false;

    // Khởi tạo lại các khối brick
    int brickWidth = 56;
    int brickHeight = 20;
    int rows = 3;
    int cols = 16;
    int startX = 10;
    int startY = 50;

    m_bricks.clear();  // Xóa các khối cũ nếu có
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            CRect brick(startX + j * (brickWidth + 5), startY + i * (brickHeight + 5),
                startX + j * (brickWidth + 5) + brickWidth,
                startY + i * (brickHeight + 5) + brickHeight);
            m_bricks.push_back(brick);
        }
    }

    SetTimer(1, 30, NULL); // Bắt đầu vòng lặp game
}


void CBrickBreaker3View::OnReplay()
{
    ReplayGame();
}

void CBrickBreaker3View::ReplayGame()
{
    m_paddle = CRect(200, 400, 300, 420);
    m_ball = CPoint(250, 200);
    m_ballSpeed = CSize(5, -5);
    m_gameRunning = false;
    m_gameStarted = false;
    m_score = 0;

    // Đặt lại trạng thái di chuyển thanh ngang
    m_moveLeft = false;
    m_moveRight = false;

    // Tạo lại các khối vuông
    m_bricks.clear();
    int brickWidth = 56, brickHeight = 20, rows = 3, cols = 16, startX = 10, startY = 50;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            CRect brick(startX + j * (brickWidth + 5), startY + i * (brickHeight + 5),
                startX + j * (brickWidth + 5) + brickWidth,
                startY + i * (brickHeight + 5) + brickHeight);
            m_bricks.push_back(brick);
        }
    }

    Invalidate();
}

void CBrickBreaker3View::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1 && m_gameRunning)
        UpdateGame();

    CView::OnTimer(nIDEvent);
}

void CBrickBreaker3View::UpdateGame()
{
    if (!m_gameRunning) return;

    // Di chuyển thanh ngang
    if (m_moveLeft && m_paddle.left > 0)
        m_paddle.OffsetRect(-m_paddleSpeed, 0);
    if (m_moveRight && m_paddle.right < 1000)
        m_paddle.OffsetRect(m_paddleSpeed, 0);

    // Di chuyển bóng
    m_ball += m_ballSpeed;

    // Xử lý va chạm bóng với tường
    if (m_ball.x - m_ballRadius <= 0 || m_ball.x + m_ballRadius >= 1000)
        m_ballSpeed.cx = -m_ballSpeed.cx;
    if (m_ball.y - m_ballRadius <= 0)
        m_ballSpeed.cy = -m_ballSpeed.cy;

    // Xử lý va chạm bóng với thanh ngang
    if (m_ball.y + m_ballRadius >= m_paddle.top &&
        m_ball.x >= m_paddle.left && m_ball.x <= m_paddle.right)
    {
        m_ballSpeed.cy = -m_ballSpeed.cy;
    }

    // Xử lý va chạm bóng với các khối
    for (auto it = m_bricks.begin(); it != m_bricks.end(); )
    {
        if (m_ball.x + m_ballRadius > it->left && m_ball.x - m_ballRadius < it->right &&
            m_ball.y + m_ballRadius > it->top && m_ball.y - m_ballRadius < it->bottom)
        {
            m_ballSpeed.cy = -m_ballSpeed.cy;
            m_score++;
            it = m_bricks.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Kiểm tra nếu thắng
    if (m_bricks.empty())
    {
        m_gameRunning = false;
        ShowWinDialog();  // Hiển thị thông báo thắng
    }

    // Kiểm tra nếu thua
    if (m_ball.y + m_ballRadius > 420)
    {
        if (m_score > m_highScore)
        {
            m_highScore = m_score;
        }
        m_gameRunning = false;
        ShowGameOverDialog();  // Hiển thị thông báo thua
    }

    Invalidate();
}

void CBrickBreaker3View::ShowGameOverDialog()
{
    CString message;
    message.Format(_T("Bạn đã thua! Nhấn OK để bắt đầu lại.\nĐiểm hiện tại: %d\nĐiểm số cao nhất: %d"), m_score, m_highScore);

    // Hiển thị hộp thoại thông báo
    AfxMessageBox(message, MB_OK | MB_ICONINFORMATION);

    // Hiển thị lại nút Start
    m_startButton.ShowWindow(SW_SHOW);
}

void CBrickBreaker3View::ShowWinDialog()
{
    CString message;
    message.Format(_T("Chúc mừng! Bạn đã thắng!\nĐiểm hiện tại: %d\nĐiểm số cao nhất: %d"), m_score, m_highScore);

    // Hiển thị hộp thoại thông báo
    AfxMessageBox(message, MB_OK | MB_ICONINFORMATION);

    // Hiển thị lại nút Start để chơi lại
    m_startButton.ShowWindow(SW_SHOW);
}

BOOL CBrickBreaker3View::PreCreateWindow(CREATESTRUCT& cs)
{
    // Cấu hình cửa sổ ở đây (nếu cần). Nếu không, chỉ trả về TRUE.
    return CView::PreCreateWindow(cs);
}

// Định nghĩa OnInitialUpdate

void CBrickBreaker3View::OnInitialUpdate()
{
    CView::OnInitialUpdate(); // Gọi hàm cơ sở

    // Tạo nút Start
    m_startButton.Create(
        _T("Start"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(450, 200, 550, 250), this, ID_GAME_START);
}

CBrickBreaker3Doc* CBrickBreaker3View::GetDocument() const
{
    return reinterpret_cast<CBrickBreaker3Doc*>(m_pDocument);
}
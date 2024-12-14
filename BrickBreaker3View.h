#pragma once

#include "afxwin.h"
#include <vector>

class CBrickBreaker3Doc;

class CBrickBreaker3View : public CView
{
protected:
    CBrickBreaker3View() noexcept;
    DECLARE_DYNCREATE(CBrickBreaker3View)

public:
    CBrickBreaker3Doc* GetDocument() const;

    virtual void OnDraw(CDC* pDC);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual ~CBrickBreaker3View();

    void UpdateGame();
    void ReplayGame();

protected:
    CButton m_startButton; // Khai báo nút Start
    DECLARE_MESSAGE_MAP()

private:
    CRect m_paddle;            // Vị trí thanh ngang
    CPoint m_ball;             // Vị trí bóng
    CSize m_ballSpeed;         // Tốc độ bóng
    int m_ballRadius;          // Bán kính bóng
    int m_paddleSpeed;         // Tốc độ di chuyển thanh ngang
    bool m_gameRunning;        // Trạng thái game đang chạy
    bool m_gameStarted;        // Đã bắt đầu game hay chưa
    bool m_moveLeft;           // Di chuyển trái
    bool m_moveRight;          // Di chuyển phải
    int m_score;               // Điểm số
    int m_highScore; // Điểm số cao nhất

    std::vector<CRect> m_bricks; // Danh sách các khối vuông
    void ShowGameOverDialog();  // Khai báo hàm để hiển thị thông báo khi thua
    void  ShowWinDialog();
public:
    afx_msg void OnStartGame(); // Khai báo hàm xử lý nút Start
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnReplay();   // Hàm xử lý nút Replay
};
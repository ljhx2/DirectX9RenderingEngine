#include "stdafx.h"
#include "DockHostImpl.h"
#include "Res\resource.h"

static HBITMAP	g_hCaptionBG = NULL;
static HBITMAP	g_hCloseButton = NULL;
static HBITMAP	g_hCloseButtonHot = NULL;
static HFONT	g_hCaptionFont = NULL;


BOOL LoadResImages(HWND hWnd)
{
	if (g_hCaptionBG)
		return TRUE;

	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
	g_hCaptionBG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DH_CAPTION));
	g_hCloseButton = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DH_CLOSE));
	g_hCloseButtonHot = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DH_CLOSE_HOT));

	// Create the caption font.
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
	g_hCaptionFont = CreateFontIndirect(&ncm.lfSmCaptionFont);

	return g_hCaptionBG && g_hCloseButton && g_hCloseButtonHot;
}


VOID CalculateDockedCaptionRects(HWND hWnd, const RECT& rNode,
	RECT* prCaption,
	RECT* prText,
	RECT* prIcon,
	RECT* prClose)
{
	//SIZE closeButtonSize = {0,0};
	if (!LoadResImages(hWnd))
		return;

	RECT rCaption, rClose;

	rCaption = rNode;
	rCaption.bottom = rCaption.top + GetBitmapSize(g_hCaptionBG).cy;

	int frameWidth = GetSystemMetrics(SM_CXFIXEDFRAME);
	int frameHeight = GetSystemMetrics(SM_CYFIXEDFRAME);

	rClose = rCaption;
	rClose.right = rNode.right - frameWidth;
	rClose.left = rClose.right - GetBitmapSize(g_hCloseButton).cx;

	if (prCaption) *prCaption = rCaption;
	if (prClose) *prClose = rClose;

	// The icon rectangle.
	if (prIcon)
	{
		HICON hIcon = (HICON)SendMessage(hWnd, WM_GETICON, ICON_SMALL, 0);
		if (hIcon)
			SetRect(prIcon, rCaption.left, rCaption.top, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));
		else
			SetRect(prIcon, 0, 0, 0, 0);
		OffsetRect(prIcon, frameWidth, frameHeight);
	}

	// The caption text rectangle.
	if (prText)
	{
		*prText = rCaption;
		prText->left += prIcon ? prIcon->right : frameWidth;
		prText->top += frameHeight;
		prText->right = rClose.left;
	}

}



BOOL DrawDockedCaption(HWND hWnd, HDC hdc, const RECT& rClient, BOOL bActive, BOOL bCloseHot)
{
	RECT rCaption, rText, rClose, rIcon;
	CalculateDockedCaptionRects(hWnd, rClient, &rCaption, &rText, &rIcon, &rClose);

	//DebugPrintf( TEXT("rCaption: %d, %d, %d, %d"), rCaption.left, rCaption.top, rCaption.right, rCaption.bottom );

	if (!LoadResImages(hWnd))
		return FALSE;

	DrawBitmap(hdc, g_hCaptionBG, rCaption.left, rCaption.top, RectWidth(rCaption), RectHeight(rCaption));
	//FillRect( hdc, &rCaption, RGB(100,100,255) );
	DrawBitmap(hdc, bCloseHot ? g_hCloseButtonHot : g_hCloseButton,
		rClose.left, rClose.top, RectWidth(rClose), RectHeight(rClose));


	// Draw the caption text.
	{
		TCHAR sText[128];
		GetWindowText(hWnd, sText, 128);

		if (g_hCaptionFont)
		{
			//COLORREF oldTextColor = SetTextColor( hdc, GetSysColor( bActive ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT ) );
			COLORREF oldTextColor = SetTextColor(hdc, RGB(255, 255, 255));
			INT oldMode = SetBkMode(hdc, TRANSPARENT);
			HFONT hOldFont = (HFONT)SelectObject(hdc, g_hCaptionFont);

			DrawText(hdc, sText, -1, &rText, DT_SINGLELINE | DT_TOP | DT_CENTER | DT_END_ELLIPSIS);

			SelectObject(hdc, hOldFont);
			SetTextColor(hdc, oldTextColor);
			SetBkMode(hdc, oldMode);
		}
	}

	return TRUE;
}

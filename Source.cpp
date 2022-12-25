#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "cObject.h"
#include "cMap.h"
#include "Utils.h"

enum ObjectState : uint32_t
{
	Stay,
	Move
};

enum ObjectDir : uint32_t
{
	None,
	Up,
	Down,
	Left,
	Right
};

enum ObjectId : uint32_t
{
	Player,
	John,
	SmartJohn,
	Lola,
	Amber,
	Ann,
	Phil,
	Sam
};

enum MapTile : uint32_t
{
	Grass,
	Tree
};

class RPG : public olc::PixelGameEngine
{
public:
	RPG()
	{
		sAppName = "RPG";
	}

public:
	std::string GetName(uint32_t nObjectId)
	{
		switch (nObjectId)
		{
		case Player:	return sPlayerName;
		case John:		return "John";
		case SmartJohn: return "Smart John";
		case Lola:		return "Lola";
		case Amber:		return "Amber";
		case Ann:		return "Ann";
		case Phil:		return "Phil";
		case Sam:		return "Sam";
		}
	}

	bool OnUserCreate() override
	{
		gfxNpc.Load("gfx/npc.png");

		vecObjects.push_back(cObject(ObjectId::Player, olc::vf2d(16.0f, 128.0f), ObjectState::Stay, ObjectDir::Right, 50.0f, 3));
		vecObjects[0].vecDialogs.push_back(std::vector<Dialog>());
		vecObjects[0].vecDialogs[0].push_back(Dialog("What's your name?", false, Lola));
		vecObjects[0].vecDialogs[0].push_back(Dialog("", true, Player));
		vecObjects[0].vecDialogs[0].push_back(Dialog("", false, Lola));

		vecObjects.push_back(cObject(ObjectId::Lola, olc::vf2d(48.0f, 128.0f), ObjectState::Stay, ObjectDir::Left, 50.0f, 3));

		mWorld.nWidth = 14;
		mWorld.nHeight = 14;

		mWorld.vecUnits.resize(mWorld.nWidth * mWorld.nHeight);

		mWorld.gfxMap.Load("gfx/map.png");

		for (size_t x = 0; x < mWorld.nWidth / 2; x++)
			for (size_t y = 0; y < mWorld.nHeight / 2; y++)
			{
				size_t i = y * mWorld.nWidth + x;
				mWorld.vecUnits[i].nId = Grass;
			}

		for (size_t x = mWorld.nWidth / 2; x < mWorld.nWidth; x++)
			for (size_t y = 0; y < mWorld.nHeight; y++)
			{
				size_t i = y * mWorld.nWidth + x;
				mWorld.vecUnits[i].nId = Tree;
			}

		for (size_t x = mWorld.nWidth / 2; x < mWorld.nWidth; x++)
			for (size_t y = mWorld.nHeight / 2; y < mWorld.nHeight; y++)
			{
				size_t i = y * mWorld.nWidth + x;
				mWorld.vecUnits[i].nId = Grass;
			}

		for (size_t x = 0; x < mWorld.nWidth / 2; x++)
			for (size_t y = mWorld.nHeight / 2; y < mWorld.nHeight; y++)
			{
				size_t i = y * mWorld.nWidth + x;
				mWorld.vecUnits[i].nId = Tree;
			}

		vFitOn = GetScreenSize() / vUnitSize;

		return true;
	}

	void DrawMapUnit(const olc::vf2d& vPos, cMap::Unit& u)
	{
		switch (u.nId)
		{
		case Grass: DrawPartialDecal(vPos * vUnitSize, mWorld.gfxMap.Decal(), olc::vf2d(0.0f, 0.0f), vUnitSize); return;
		case Tree: DrawPartialDecal(vPos * vUnitSize, mWorld.gfxMap.Decal(), vUnitSize * olc::vf2d(0.0f, 1.0f), vUnitSize); return;
		}
	}

	void ApplyOffsetNPC(uint32_t nId, olc::vf2d& vOffset)
	{
		switch (nId)
		{
		case ObjectId::Player: vOffset = { 0.0f, 0.0f }; return;
		case ObjectId::Amber: vOffset = vUnitSize * olc::vf2d(3.0f, 0.0f); return;
		case ObjectId::Ann: vOffset = vUnitSize * olc::vf2d(6.0f, 0.0f); return;
		case ObjectId::Phil: vOffset = vUnitSize * olc::vf2d(9.0f, 0.0f); return;
		case ObjectId::Lola: vOffset = vUnitSize * olc::vf2d(0.0f, 4.0f); return;
		case ObjectId::John: vOffset = vUnitSize * olc::vf2d(3.0f, 4.0f); return;
		case ObjectId::SmartJohn: vOffset = vUnitSize * olc::vf2d(6.0f, 4.0f); return;
		case ObjectId::Sam: vOffset = vUnitSize * olc::vf2d(9.0f, 4.0f); return;
		}
	}

	void ApplyOffsetDirection(uint32_t nDir, int32_t nFrame, olc::vf2d& vOffset)
	{
		switch (nDir)
		{
		case ObjectDir::Down: vOffset = { (float)nFrame * vUnitSize.x, fMoveDownOffset };	return;
		case ObjectDir::Left: vOffset = { (float)nFrame * vUnitSize.x, fMoveLeftOffset };	return;
		case ObjectDir::Right: vOffset = { (float)nFrame * vUnitSize.x, fMoveRightOffset }; return;
		case ObjectDir::Up: vOffset = { (float)nFrame * vUnitSize.x, fMoveUpOffset };		return;
		}
	}

	void ShowDialog(size_t nGroupIndex, size_t nDialogIndex, cObject& o)
	{
		if (bTextEntryComplete || !o.vecDialogs[nGroupIndex][nDialogIndex].bEntry)
		{
			if (o.vecDialogs[nGroupIndex][nDialogIndex].bEntry)
				o.vecDialogs[nGroupIndex][nDialogIndex].bConfirmed = true;

			o.vecDialogs[nGroupIndex][nDialogIndex].bFinished = true;
			bTextEntryComplete = false;
		}

		if (o.vecDialogs[nGroupIndex][nDialogIndex].bEntry && !o.vecDialogs[nGroupIndex][nDialogIndex].bFinished)
		{
			if (IsTextEntryEnabled())
				o.vecDialogs[nGroupIndex][nDialogIndex].sText = TextEntryGetString();
			else
				TextEntryEnable(true);
		}
		
		FillRectDecal(olc::vf2d(1.0f, 1.0f), olc::vf2d((float)ScreenWidth() - 2.0f, 40.0f), olc::BLUE);
		DrawRectDecal(olc::vf2d(1.0f, 1.0f), olc::vf2d((float)ScreenWidth() - 2.0f, 40.0f), olc::WHITE);
		DrawStringDecal(olc::vf2d(5.0f, 5.0f), GetName(o.vecDialogs[nGroupIndex][nDialogIndex].nAuthorId) + ": " + o.vecDialogs[nGroupIndex][nDialogIndex].sText, olc::WHITE);
	}

	void ShowDialogGroup(size_t nGroupIndex, cObject& o)
	{
		for (size_t i = 0; i < o.vecDialogs[nGroupIndex].size(); i++)
		{
			if (GetKey(olc::Key::ENTER).bPressed && !o.vecDialogs[nGroupIndex][i].bConfirmed)
				o.vecDialogs[nGroupIndex][i].bConfirmed = o.vecDialogs[nGroupIndex][i].bFinished;

			if (!o.vecDialogs[nGroupIndex][i].bConfirmed)
			{
				ShowDialog(nGroupIndex, i, o);
				return;
			}
		}
	}

	void DrawObject(const cObject& o)
	{
		olc::vf2d vOffsetNpc;
		ApplyOffsetNPC(o.nId, vOffsetNpc);

		if (vecObjects[0].vMapOffset == o.vMapOffset)
		{
			switch (o.nState)
			{
			case ObjectState::Stay:
			{
				switch (o.nDir)
				{
				case ObjectDir::Down: DrawPartialDecal(o.vMapPos, vUnitSize, gfxNpc.Decal(), vOffsetNpc + olc::vf2d(vUnitSize.x, 0.0f), vUnitSize);					return;
				case ObjectDir::Left: DrawPartialDecal(o.vMapPos, vUnitSize, gfxNpc.Decal(), vOffsetNpc + olc::vf2d(vUnitSize.x, vUnitSize.y), vUnitSize);			return;
				case ObjectDir::Right: DrawPartialDecal(o.vMapPos, vUnitSize, gfxNpc.Decal(), vOffsetNpc + olc::vf2d(vUnitSize.x, vUnitSize.y * 2.0f), vUnitSize);	return;
				case ObjectDir::Up: DrawPartialDecal(o.vMapPos, vUnitSize, gfxNpc.Decal(), vOffsetNpc + olc::vf2d(vUnitSize.x, vUnitSize.y * 3.0f), vUnitSize);		return;
				}
			}
			return;

			case ObjectState::Move:
			{
				olc::vf2d vOffsetDirection;
				ApplyOffsetDirection(o.nDir, o.nCurrentFrame, vOffsetDirection);

				switch (o.nDir)
				{
				case ObjectDir::Down: DrawPartialDecal(o.vMapPos, vUnitSize, gfxNpc.Decal(), vOffsetNpc + vOffsetDirection, vUnitSize);	 return;
				case ObjectDir::Left: DrawPartialDecal(o.vMapPos, vUnitSize, gfxNpc.Decal(), vOffsetNpc + vOffsetDirection, vUnitSize);	 return;
				case ObjectDir::Right: DrawPartialDecal(o.vMapPos, vUnitSize, gfxNpc.Decal(), vOffsetNpc + vOffsetDirection, vUnitSize); return;
				case ObjectDir::Up: DrawPartialDecal(o.vMapPos, vUnitSize, gfxNpc.Decal(), vOffsetNpc + vOffsetDirection, vUnitSize);	 return;
				}
			}
			return;

			}
		}
	}

	void Move(cObject& o, float fElapsedTime, uint32_t nDir)
	{
		switch (nDir)
		{
		case ObjectDir::Right: o.vMapPos.x += o.fSpeed * fElapsedTime; break;
		case ObjectDir::Left: o.vMapPos.x -= o.fSpeed * fElapsedTime;  break;
		case ObjectDir::Up: o.vMapPos.y -= o.fSpeed * fElapsedTime;	   break;
		case ObjectDir::Down: o.vMapPos.y += o.fSpeed * fElapsedTime;  break;
		case ObjectDir::None:
		{
			o.nState = ObjectState::Stay;
			o.nCurrentFrame = 1;
		}
		break;
		}

		if (nDir != ObjectDir::None)
		{
			o.nState = ObjectState::Move;
			o.nDir = nDir;
		}
	}

	void UpdateObject(cObject& o, float fElapsedTime, uint32_t nDir, bool bUpdate = true)
	{
		if (!bUpdate)
			return;

		UpdateFrame(o, fElapsedTime);
		Move(o, fElapsedTime, nDir);

		if (o.vMapPos.x >= (float)ScreenWidth())
		{
			o.vMapPos.x = 0.0f;
			o.vMapOffset.x++;
		}

		if (o.vMapPos.y >= (float)ScreenHeight())
		{
			o.vMapPos.y = 0.0f;
			o.vMapOffset.y++;
		}

		if (o.vMapPos.x < -vUnitSize.x)
		{
			o.vMapPos.x = (float)ScreenWidth() - vUnitSize.x;
			o.vMapOffset.x--;
		}

		if (o.vMapPos.y < -vUnitSize.y)
		{
			o.vMapPos.y = (float)ScreenHeight() - vUnitSize.y;
			o.vMapOffset.y--;
		}

		if (o.vMapOffset > vMaxMapOffset)
			o.vMapOffset = vMaxMapOffset;

		if (o.vMapOffset < vMinMapOffset)
			o.vMapOffset = vMinMapOffset;
	}

	void UpdateFrame(cObject& o, float fElapsedTime)
	{
		o.fFrameTime += o.fSpeed * fElapsedTime;

		if (o.fFrameTime >= o.fSpeed / 10.0f)
		{
			o.nCurrentFrame++;
			o.fFrameTime = 0.0f;
		}

		Wrap<int32_t>(0, (int32_t)o.nFrameCount - 1, o.nCurrentFrame);
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::Key::LEFT).bHeld)
			UpdateObject(vecObjects[0], fElapsedTime, ObjectDir::Left);
		else if (GetKey(olc::Key::RIGHT).bHeld)
			UpdateObject(vecObjects[0], fElapsedTime, ObjectDir::Right);
		else if (GetKey(olc::Key::UP).bHeld)
			UpdateObject(vecObjects[0], fElapsedTime, ObjectDir::Up);
		else if (GetKey(olc::Key::DOWN).bHeld)
			UpdateObject(vecObjects[0], fElapsedTime, ObjectDir::Down);
		else
			UpdateObject(vecObjects[0], fElapsedTime, ObjectDir::None);

		for (uint32_t x = 0; x < (uint32_t)vFitOn.x; x++)
			for (uint32_t y = 0; y < (uint32_t)vFitOn.y; y++)
				DrawMapUnit(olc::vf2d(x, y), mWorld.vecUnits[(y + vFitOn.y * vecObjects[0].vMapOffset.y) * mWorld.nWidth + (x + vFitOn.x * vecObjects[0].vMapOffset.x)]);

		for (const auto& o : vecObjects)
			DrawObject(o);

		ShowDialogGroup(0, vecObjects[0]);

		if (vecObjects[0].vecDialogs[0][1].bFinished)
		{
			sPlayerName = vecObjects[0].vecDialogs[0][1].sText;

			vecObjects[0].vecDialogs[0][2].sText =
				"Hello, " + sPlayerName + '!';
		}

		if (!vecObjects[0].vecDialogs[0][2].bConsequenceDone)
		{
			if (vecObjects[0].vecDialogs[0][2].bConfirmed)
			{
				UpdateObject(vecObjects[1], fElapsedTime, ObjectDir::Right);

				if (vecObjects[1].vMapOffset.x > 0)
				{
					vecObjects[0].vecDialogs[0][2].bConsequenceDone = true;
					vecObjects.erase(vecObjects.begin() + 1);
				}
			}
		}

		return true;
	}

	void OnTextEntryComplete(const std::string& sText) override
	{
		UNUSED(sText);
		bTextEntryComplete = true;
	}

private:
	cMap mWorld;
	olc::Renderable gfxNpc;

	bool bTextEntryComplete = false;
	std::vector<cObject> vecObjects;

	std::string sPlayerName = "You";

	olc::vu2d vMinMapOffset = { 0, 0 };
	olc::vu2d vMaxMapOffset = { 1, 1 };

	olc::vf2d vUnitSize = { 32.0f, 32.0f };
	olc::vf2d vFitOn = { 0.0f, 0.0f };

	float fMoveDownOffset = 0.0f;
	float fMoveLeftOffset = vUnitSize.y;
	float fMoveRightOffset = vUnitSize.y * 2.0f;
	float fMoveUpOffset = vUnitSize.y * 3.0f;

};

int main()
{
	RPG demo;

	if (demo.Construct(224, 224, 4, 4))
		demo.Start();

	return 0;
}

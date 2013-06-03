#include <sstream>
#include <string>
#include <vector>
#include "Core/GTDependencies.h"
#include "Core/GTAssets.h"
#include "Core/GTGame.h"
#include "Utils/GTUtils.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvuj;
using namespace sses;
using namespace ssvs;
using namespace ssvsc;
using namespace ssvsc::Utils;

namespace gt
{
	Color getRandomColor() { return Color(getRnd(0, 255), getRnd(0, 255), getRnd(0, 255), 255); }

	GTShape::GTShape(Vector2f mMin, Vector2f mMax, Color mColor) : min{mMin}, max{mMax}, color{mColor}
	{
		rectangleShape.setOrigin({0.f, 0.f});
		rectangleShape.setPosition(min);
		rectangleShape.setSize(max - min);
		rectangleShape.setFillColor(color);
		rectangleShape.setOutlineColor(Color::Black);
		rectangleShape.setOutlineThickness(5.1f);
	}
	void GTShape::update()
	{
		if(opacity < 254) opacity += 2;
		else opacity = 255;
		rectangleShape.setFillColor(Color(color.r, color.g, color.b, opacity));
	}
	GTShape GTShape::getSubShape(float mMinWidth, float mMinHeight, float mMaxWidth, float mMaxHeight, Color mColor)
	{
		//float parentWidth{max.x - min.x};
		float width(getRnd(mMinWidth, mMaxWidth));
		float xMin{min.x + getRnd(0, mMaxWidth - width)}, xMax{max.x - mMaxWidth};
		float x(getRnd(xMin, xMax));

		//float parentHeight{max.y - min.y};
		float height(getRnd(mMinHeight, mMaxHeight));
		float yMin{min.y + getRnd(0, mMaxHeight - height)}, yMax{max.y - mMaxHeight};
		float y(getRnd(yMin, yMax));

		return GTShape{{x, y}, {x + width, y + height}, mColor};
	}

	GTGame::GTGame(GameWindow& mGameWindow, GTAssets& mAssets) : gameWindow(mGameWindow), assets(mAssets)
	{
		for(const auto& t : assets.getAssetManager().getTextures()) t.second->setSmooth(true);

		gameState.onUpdate += [&](float mFrameTime){ update(mFrameTime); };
		gameState.onDraw += [&]{ draw(); };

		camera.zoom(3.f);
		camera.centerOn({640, 480});

		initInput();
	}

	void GTGame::restart()
	{
		shapes.clear();
		shapeTimeline.clear();
		shapeTimeline.reset();

		Color canvasColor{Color::White};
		float canvasX{0}, canvasY{0}, canvasWidth{1440}, canvasHeight{900};
		GTShape* canvas{new GTShape{{canvasX, canvasY}, {canvasX + canvasWidth, canvasY + canvasHeight}, canvasColor}};

		currentCanvas = canvas;
		shapes.push_back(*canvas);

		shapeTimeline.append<Do>([&, canvas, canvasColor]{ shapes.push_back(canvas->getSubShape(150, 150, 900, 600, canvasColor)); });
		shapeTimeline.append<Wait>(25);
		shapeTimeline.append<Do>([&, canvas, canvasColor]{ shapes.push_back(canvas->getSubShape(150, 150, 900, 600, canvasColor)); });
		shapeTimeline.append<Wait>(25);
		shapeTimeline.append<Do>([&, canvas, canvasColor]{ shapes.push_back(canvas->getSubShape(150, 150, 900, 600, canvasColor)); });
		shapeTimeline.append<Wait>(25);
		shapeTimeline.append<Do>([&, canvas, canvasColor]{ shapes.push_back(canvas->getSubShape(150, 150, 900, 600, canvasColor)); });
		shapeTimeline.append<Wait>(25);
		shapeTimeline.append<Do>([&, canvas]{ shapes.push_back(canvas->getSubShape(500, 300, 900, 350, getRandomColor())); });
		shapeTimeline.append<Wait>(25);
		shapeTimeline.append<Do>([&, canvas]{ shapes.push_back(canvas->getSubShape(100, 300, 200, 350, getRandomColor())); });
		shapeTimeline.append<Wait>(25);
		shapeTimeline.append<Do>([&, canvas]{ shapes.push_back(canvas->getSubShape(100, 300, 200, 350, getRandomColor())); });
		shapeTimeline.append<Wait>(25);
		shapeTimeline.append<Do>([&, canvas]{ shapes.push_back(canvas->getSubShape(100, 300, 200, 350, getRandomColor())); });
	}

	void GTGame::initInput()
	{
		using k = Keyboard::Key;
		//using b = Mouse::Button;
		//using t = Input::Trigger::Types;

		gameState.addInput({{k::Escape}}, [&](float){ gameWindow.stop(); });
		gameState.addInput({{k::R}}, [&](float){ restart(); });

		float spd = 8.0f;
		gameState.addInput({{k::A}}, [=](float){ camera.move(Vector2f{-spd, 0} * getRealFT()); });
		gameState.addInput({{k::D}}, [=](float){ camera.move(Vector2f{spd, 0} * getRealFT()); });
		gameState.addInput({{k::W}}, [=](float){ camera.move(Vector2f{0, -spd} * getRealFT()); });
		gameState.addInput({{k::S}}, [=](float){ camera.move(Vector2f{0, spd} * getRealFT()); });
		gameState.addInput({{k::Q}}, [=](float){ camera.zoom(1.f + 0.02f * getRealFT()); });
		gameState.addInput({{k::E}}, [=](float){ camera.zoom(1.f - 0.02f * getRealFT()); });

	}

	void GTGame::update(float mFrameTime)
	{
		shapeTimeline.update(mFrameTime);

		if(currentCanvas != nullptr && timer >= timerMax)
		{
			shapeTimeline.append<Do>([&]{ shapes.push_back(currentCanvas->getSubShape(100, 100, 900, 650, Color::White)); });
			shapeTimeline.append<Wait>(135);
			shapeTimeline.append<Do>([&]{ shapes.push_back(currentCanvas->getSubShape(100, 100, 900, 650, getRandomColor())); });
			shapeTimeline.start();
			timer = 0;
		}
		else timer += mFrameTime;

		for(auto& s : shapes) s.update();
	}
	void GTGame::draw()
	{
		camera.apply();
		for(const auto& s : shapes) render(s.rectangleShape);
		camera.unapply();
	}
	void GTGame::render(const Drawable& mDrawable) { gameWindow.draw(mDrawable); }

	// Getters
	GameWindow& GTGame::getGameWindow()	{ return gameWindow; }
	GameState& GTGame::getGameState()	{ return gameState; }
	Vector2f GTGame::getMousePosition()	{ return camera.getMousePosition(); }
	float GTGame::getRealFT()			{ return 60.f / gameWindow.getFPS(); }





}

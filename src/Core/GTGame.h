// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVGT_CORE_GAME
#define SSVGT_CORE_GAME

#include "Core/GTDependencies.h"

namespace gt
{
	constexpr int cameraWidth{1440};
	constexpr int cameraHeight{900};

	class GTAssets;

	struct GTShape
	{
		sf::Vector2f min, max;
		sf::Color color;
		sf::RectangleShape rectangleShape;
		float opacity{0};

		GTShape(sf::Vector2f mMin, sf::Vector2f mMax, sf::Color mColor);

		void update();
		GTShape getSubShape(float mMinWidth, float mMinHeight, float mMaxWidth, float mMaxHeight, sf::Color mColor);
	};

	class GTGame
	{
		private:
			ssvs::GameWindow& gameWindow;
			ssvs::Camera camera{gameWindow, {{320 / 2, 240 / 2}, {cameraWidth / 3, cameraHeight / 3}}};
			GTAssets& assets;
			ssvs::GameState gameState;
			std::vector<GTShape> shapes;
			GTShape* currentCanvas{nullptr};
			ssvu::Timeline shapeTimeline;

			float timer{0};
			float timerMax{300};

			void initInput();
			void restart();

		public:
			GTGame(ssvs::GameWindow& mGameWindow, GTAssets& mAssets);

			void update(float mFrameTime);
			void draw();

			void render(const sf::Drawable& mDrawable);

			// Getters
			ssvs::GameWindow& getGameWindow();
			ssvs::GameState& getGameState();
			sf::Vector2f getMousePosition();
			float getRealFT();
	};
}

#endif

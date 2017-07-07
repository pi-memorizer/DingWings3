#include "GameState.h"
#include "Entity.h"
#include "Player.h"
#include "World.h"
#include "Sprite.h"
#include "Event.h"
#include "Animation.h"

GameState::GameState(Player *player)
{
	p = player;
	a = true;
	b = true;
	up = true;
	down = true;
	left = true;
	right = true;
}

void GameState::startMenu()
{

}

void GameState::endMenu()
{
	a = getKey(p,KEY_A);
	b = getKey(p,KEY_B);
	up = getKey(p,KEY_UP);
	down = getKey(p,KEY_DOWN);
	left = getKey(p,KEY_LEFT);
	right = getKey(p,KEY_RIGHT);
}

WorldState::WorldState(Player *player) : GameState(player)
{

}

void WorldState::draw()
{
	setDrawColor(0xFF, 0xFF, 0xFF, 0);
	clearScreen();
	for (int i = 0; i < animations.length(); i++)
	{
		if (!animations[i]->isDone()) animations[i]->draw();
	}
	World *world = worlds[p->getWorldID()];
	for (int i = p->getCameraCenterX() - WIDTH / TILE_SIZE / 2 - 2; i <= p->getCameraCenterX() + WIDTH / TILE_SIZE / 2 + 2; i++)
	{
		for (int j = p->getCameraCenterY() - HEIGHT / TILE_SIZE / 2 - 2; j <= p->getCameraCenterY() + HEIGHT / TILE_SIZE / 2 + 2; j++)
		{
			int index = world->getLower(i, j);
			if (tileset[index] != nullptr&&index != 0) tileset[index]->draw(getOnscreenX(p, i, 0), getOnscreenY(p, j, 0));
		}
	}
	List<Entity*> &entities = worlds[p->getWorldID()]->entities;
	for (int i = 0; i < entities.length(); i++)
	{
		Entity *e = entities[i];
		assert(e != nullptr);
		if (e->isAlive)
		{
			if(e->sprite!=nullptr) e->sprite->draw(getOnscreenX(p, e->x, e->xOffset), getOnscreenY(p, e->y, e->yOffset));
		}
	}
	for (int i = 0; i < numPlayers; i++)
	{
		players[i]->getSprite()->draw(getOnscreenX(p, players[i]->x, players[i]->xOffset), getOnscreenY(p, players[i]->y, players[i]->yOffset));
	}
	for (int i = p->getCameraCenterX() - WIDTH / TILE_SIZE / 2 - 2; i <= p->getCameraCenterX() + WIDTH / TILE_SIZE / 2 + 2; i++)
	{
		for (int j = p->getCameraCenterY() - HEIGHT / TILE_SIZE / 2 - 2; j <= p->getCameraCenterY() + HEIGHT / TILE_SIZE / 2 + 2; j++)
		{
			int index = world->getUpper(i, j);
			if (tileset[index] != nullptr&&index != 0) tileset[index]->draw(getOnscreenX(p, i, 0), getOnscreenY(p, j, 0));
		}
	}
}

void offsetShift(Player *p)
{
	while (p->xOffset <= -TILE_SIZE)
	{
		p->xOffset += TILE_SIZE;
		p->x--;
	}
	while (p->yOffset <= -TILE_SIZE)
	{
		p->yOffset += TILE_SIZE;
		p->y--;
	}
	while (p->xOffset >= TILE_SIZE)
	{
		p->xOffset -= TILE_SIZE;
		p->x++;
	}
	while (p->yOffset >= TILE_SIZE)
	{
		p->yOffset -= TILE_SIZE;
		p->y++;
	}
}

void attemptMove(Player *p, int dx, int dy, int speed)
{
	for (int i = 0; i < speed; i++)
	{
		p->xOffset += dx;
		p->yOffset += dy;
		offsetShift(p);
		if (worlds[p->getWorldID()]->collides(p->x, p->y, p->xOffset, p->yOffset, p->width, p->height))
		{
			p->xOffset -= dx;
			p->yOffset -= dy;
			break;
		}
	}
	offsetShift(p);
}

bool firstWithWorldId(int worldId, int playerId)
{
	for (int i = 0; i < numPlayers; i++)
	{
		if (getPlayer(i)->getWorldID() == worldId)
			return i == playerId;
	}
	return false;
}

void WorldState::run()
{
	for (int i = 0; i < animations.length(); i++)
	{
		if (animations[i]->isDone())
		{
			animations.removeAt(i);
			i--;
		}
		else {
			animations[i]->run();
		}
	}
	startMenu();
	if (this == p->getState())
	{
		int dx = 0;
		int dy = 0;
		int _dir = p->dir;
		if (getKey(p,KEY_UP)) dy--;
		if (getKey(p,KEY_DOWN)) dy++;
		if (getKey(p,KEY_LEFT)) dx--;
		if (getKey(p,KEY_RIGHT)) dx++;
		if (p->dir == 0 && dx > 0)
		{
		}
		else if (p->dir == 1 && dy < 0)
		{
		}
		else if (p->dir == 2 && dx < 0)
		{
		}
		else if (p->dir == 3 && dy > 0)
		{
		}
		else {
			if (dx > 0)
				p->dir = 0;
			else if (dy < 0)
				p->dir = 1;
			else if (dx < 0)
				p->dir = 2;
			else if (dy > 0)
				p->dir = 3;
		}
		if (_dir != p->dir) p->wait = 0;
		if ((dx != 0 || dy != 0) && p->wait >= 3) attemptMove(p, dx, dy, 2);
		else if (dx == 0 && dy == 0)
			p->wait = 0;
		p->wait++;
		int sight = 1;
		if (getKey(p,KEY_A) && !a)
		{
			bool mapInteract = false;
			if (p->dir == 0)
			{
				if (worlds[p->getWorldID()]->interact(p, safeDiv(TILE_SIZE * p->x + p->xOffset + p->width + sight, TILE_SIZE), safeDiv(TILE_SIZE * p->y + p->yOffset + p->height / 2, TILE_SIZE)))
					mapInteract = true;
			}
			if (p->dir == 1)
			{
				if (worlds[p->getWorldID()]->interact(p, safeDiv(TILE_SIZE * p->x + p->xOffset + p->width / 2, TILE_SIZE), safeDiv(TILE_SIZE * p->y + p->yOffset - sight, TILE_SIZE)))
					mapInteract = true;
			}
			if (p->dir == 2)
			{
				if (worlds[p->getWorldID()]->interact(p, safeDiv(TILE_SIZE * p->x + p->xOffset - sight, TILE_SIZE), safeDiv(TILE_SIZE * p->y + p->yOffset + p->height / 2, TILE_SIZE)))
					mapInteract = true;
			}
			if (p->dir == 3)
			{
				if (worlds[p->getWorldID()]->interact(p, safeDiv(TILE_SIZE * p->x + p->xOffset + p->width / 2, TILE_SIZE), safeDiv(TILE_SIZE * p->y + p->yOffset + p->height + sight, TILE_SIZE)))
					mapInteract = true;
			}
			if (!mapInteract)
			{
				//try interacting with entities
				List<Entity*> entities = worlds[p->getWorldID()]->entities;
				int x = 0;
				int y = 0;
				if (p->dir == 0)
				{
					x = TILE_SIZE * p->x + p->xOffset + p->width + sight;
					y = TILE_SIZE * p->y + p->yOffset + p->height / 2;
				}
				else if (p->dir == 1)
				{
					x = TILE_SIZE * p->x + p->xOffset + p->width / 2;
					y = TILE_SIZE * p->y + p->yOffset - sight;
				}
				else if (p->dir == 2)
				{
					x = TILE_SIZE * p->x + p->xOffset - sight;
					y = TILE_SIZE * p->y + p->yOffset + p->height / 2;
				}
				else if (p->dir == 3)
				{
					x = TILE_SIZE * p->x + p->xOffset + p->width / 2;
					y = TILE_SIZE * p->y + p->yOffset + p->height + sight;
				}
				for (int i = 0; i < entities.length(); i++)
				{
					Entity *e = entities[i];
					if (e->isAlive)
					{
						if (rectCollides(x, y, 2, 2, TILE_SIZE * e->x + e->xOffset, TILE_SIZE * e->y + e->yOffset, e->width, e->height))
						{
							if (e->interact(p))
							{
								break;
							}
						}
					}
				}
			}
		}
	}
	if (firstWithWorldId(p->getWorldID(), p->id))
	{
		List<Entity*> &entities = worlds[p->getWorldID()]->entities;
		for (int i = 0; i < entities.length(); i++)
		{
			if (entities[i]->isAlive)
			{
				entities[i]->run();
			}
			if (!entities[i]->isAlive)
			{
				entities.removeAt(i);
				i--;
			}
		}
	}
	endMenu();
}
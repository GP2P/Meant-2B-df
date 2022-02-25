#include "Player.h"
#include "WorldManager.h"
#include "EventStep.h"
#include "EventKeyboard.h"
#include "Arrow.h"
#include "EventMouse.h"
#include "Stone.h"
#include "DisplayManager.h"

Player::Player() = default;

Player::Player(int ID) {
	setID(ID);
	setType("Player");
	setSolidness(df::HARD);
	if (ID == 1)
		setSprite("Player1Standing");
	else
		setSprite("Player2Standing");
	setVelocity(df::Vector());
	registerInterest(df::STEP_EVENT);
	registerInterest(df::KEYBOARD_EVENT);
	registerInterest(df::MSE_EVENT);
	acceleration = df::Vector(0, 0.1);
	jumpSpeed = 0.95;
	shootSlowdown = 30;
	shootCountdown = shootSlowdown;
	walkingCountdown = 0;
	havestone = false;
}

bool Player::onGround() {
	df::ObjectList collisions = WM.getCollisions(this, getPosition() + getVelocity());

	// soft collisions don't count as on ground
	if (!collisions.isEmpty()) {
		auto oli = df::ObjectListIterator(&collisions);
		while (!oli.isDone()) {
			if (oli.currentObject()->getSolidness() == df::HARD)
				return true;
			oli.next();
		}
		return false;
	} else return false;
}

int Player::eventHandler(const df::Event *p_e) {

	if (p_e->getType() == df::STEP_EVENT) {
		if (!onGround()) setVelocity(getVelocity() + acceleration);
		else setVelocity(df::Vector(0, 0));

		shootCountdown--;
		if (shootCountdown < 0)
			shootCountdown = 0;

		if (walkingCountdown == 0) {
			if (ID == 1)
				setSprite("Player1Standing");
			else
				setSprite("Player2Standing");
			walkingCountdown = -1;
		} else {
			walkingCountdown--;
		}
		if (havestone) {
			if (getID() == 1) {
				DM.drawString(getPosition() - df::Vector(0, 3), "Press R to drop stone", df::CENTER_JUSTIFIED,
				              df::WHITE);
			}
			if (getID() == 2) {
				DM.drawString(getPosition() - df::Vector(0, 3), "Press Right Shift to drop stone", df::CENTER_JUSTIFIED,
				              df::WHITE);
			}
		}

		return 1;

	}
	if (p_e->getType() == df::KEYBOARD_EVENT) {
		const auto *p_keyboard_event = dynamic_cast <const df::EventKeyboard *> (p_e);
		switch (p_keyboard_event->getKey()) {
			case df::Keyboard::A:    // left
				if (ID == 1) {
					if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
						if (walkingCountdown < 1 && onGround()) {
							setSprite("Player1Walking");
							walkingCountdown = 10;
						}
					WM.moveObject(this, df::Vector(getPosition().getX() - (float) 1.0, getPosition().getY()));
				}
				break;
			case df::Keyboard::LEFTARROW:    // left
				if (ID == 2) {
					if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
						if (walkingCountdown < 1 && onGround()) {
							setSprite("Player2Walking");
							walkingCountdown = 10;
						}
					WM.moveObject(this, df::Vector(getPosition().getX() - (float) 1.0, getPosition().getY()));
				}
				break;
			case df::Keyboard::D:    // right
				if (ID == 1) {
					if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
						if (walkingCountdown < 1 && onGround()) {
							setSprite("Player1Walking");
							walkingCountdown = 10;
						}
					WM.moveObject(this, df::Vector(getPosition().getX() + (float) 1.0, getPosition().getY()));
				}
				break;
			case df::Keyboard::RIGHTARROW:    // right
				if (ID == 2) {
					if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
						if (walkingCountdown < 1 && onGround()) {
							setSprite("Player2Walking");
							walkingCountdown = 10;
						}
					WM.moveObject(this, df::Vector(getPosition().getX() + (float) 1.0, getPosition().getY()));
				}
				break;
			case df::Keyboard::W:    // jump
				if (ID == 1)
					jump();
				break;
			case df::Keyboard::UPARROW:    // jump
				if (ID == 2)
					jump();
				break;
			case df::Keyboard::R:    // use item
				if (ID == 1)
					if (havestone) {
						new Stone(getPosition());
						havestone = false;
					}
				break;
			case df::Keyboard::RIGHTSHIFT:    // use item
				if (ID == 2)
					if (havestone) {
						new Stone(getPosition());
						havestone = false;
					}
				break;
			default:    // Key not included
				break;
		}
		return 1;
	}
	if (p_e->getType() == df::MSE_EVENT) {
		const auto *p_mouse_event = dynamic_cast <const df::EventMouse *> (p_e);
		if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
		    (p_mouse_event->getMouseButton() == df::Mouse::LEFT)) {
			shoot(p_mouse_event->getMousePosition());
		}
		return 1;
	}
	return 0;
}

int Player::jump() {
	if (onGround()) {
		setVelocity(df::Vector(0, -jumpSpeed));
		return 1;
	}

	return 0;
}

void Player::shoot(df::Vector target) {

	// See if time to shoot.
	if (shootCountdown > 0)
		return;
	shootCountdown = shootSlowdown;

	// Shoot Arrow towards target.
	df::Vector v = target - getPosition();
	v.normalize();
	v.scale(1);
	auto *p = new Arrow(getPosition());
	p->setVelocity(v);
}

int Player::getID() const {
	return ID;
}

void Player::setID(int id) {
	ID = id;
}

#include "ball.h"
#include "math.h"
#include "iostream"

Ball::Ball(SDL_Renderer* renderer,ImageLoader* imgLoader,Padel* padel){
	position = new Vector2D(x,y);
	calculateCenter();
	this->renderer = renderer;
	this->imgLoader = imgLoader;
	this->padel     = padel;
	this->image    = imgLoader->loadTexture("C:/Users/acosere/Desktop/Work/Game-SpaceInvaders/circle.png", renderer);
	ballStartPosition();
	radius = width/2;
}

void Ball::update(float timeDifference){
	if(timeDifference>0.017){
		timeDifference = 0.017;
	}
	checkWallCollision();
	checkPadelCircleCollision();
	checkWallCollision();
	move(timeDifference);
}

void Ball::calculateCenter(){
	center = new Vector2D(position->x + width/2, position->y + height/2); 
}


void Ball::draw(){
	imgLoader->renderTexture(image,renderer,position->x,position->y,width,height);
//	renderParticles();	
}

void Ball::move(float timeDifference){
	//speed = sqrt(x^2 + y^2)
	Vector2D* b = *velocity * timeDifference;
	position->x = position->x + b->x;
	position->y = position->y + b->y;
	center->x   = position->x + width/2;
	center->y   = position->y + height/2;
//	updateParticles(b.x,b.y);
}

float Ball::clamp(float value, float min, float max){
	if(value < min){
		return min;
	}else if(value > max){
		return max;
	}else{
		return value;
	}
	
}


//Don't forget to add penetration here too
void Ball::checkWallCollision(){
	if(position->y <= 0){
		position->y = 0;
		velocity->y = std::abs(velocity->y);
		// add penetration here also
	}
	
	if(position->x < 0){
		position->x = 0;
		velocity->x = std::abs(velocity->x);
	}
	
	if(position->x + width >= Globals::SCREEN_WIDTH){
		position->x = Globals::SCREEN_WIDTH - width;
		velocity->x = -std::abs(velocity->x);
	}
}

//rethink left/right/mid collision! include sides of the paddle or not?
void Ball::checkPadelCircleCollision(){
	clampVector->x = clamp(center->x,padel->position->x,padel->position->x+padel->barWidth);
	clampVector->y = clamp(center->y,padel->position->y,padel->position->y+padel->barHeight);
	float calculatedDistance = clampVector->distanceBetweenTwoVectors(center);
	if(calculatedDistance < radius){
		Vector2D* distanceCenterClamp = new Vector2D(clampVector->x - center->x, clampVector->y - center->y);
		Vector2D* normalize = distanceCenterClamp->normalize();
		//it's always 1
	//	std::cout<<"Normalize: "<<normalize->magnitude()<<std::endl;
		
		float max = -1.0f;
		int best_match = -1;
		float penetration = 0;
		
		for(int i=0;i<4;i++){
			float dot_product = normalize->dotProduct(compass[i]);
			if(dot_product > max){
				max = dot_product;
				best_match = i;
			}
			
			if(max > 1.0f){
				max = 1;
			}
			
			if(best_match == hitUp){
				penetration = radius - distanceCenterClamp->y;
			}
		}
			position->y = position->y - penetration;
			checkDistanceFromMiddle();

	} 
}

void Ball::checkDistanceFromMiddle(){
	float centerBoard  = padel->position->x + padel->barWidth/2; 
	float ballDistance = (position->x + radius) - centerBoard;  //we already have a collision here
	//calculate distance in px from ball center to padel center ex: -3 px from middle
	float percentage   = ballDistance /  (padel->barWidth/2);
	
	if(percentage < 0){
		if(percentage < -1.0){
			percentage = -1.0;
		}
	}else if(percentage > 0){
		if(percentage > 1.0){
			percentage = 1.0;
		}
	}
	
	
	float strength = 2.0f;
	Vector2D* oldVelocity = new Vector2D(velocity->x, velocity->y);
	velocity->x  = initialBallVelocity->x * percentage * strength;
	velocity->y = -1 * std::abs(velocity->y);
	Vector2D* normalized = velocity->normalize();
	velocity->x = normalized->x * oldVelocity->magnitude();
	velocity->y = -(speed - std::abs(velocity->x));
}

void Ball::ballStartPosition(){
	float randomXPosition = rand()%100 + 301;
	float randomYPosition = rand()%100 + 150;
	
	position->x = randomXPosition;
	position->y = randomYPosition;
}


void Ball::renderParticles(){
	//Go through particles
    for( int i = 0; i < 10; ++i )
    {
        //Delete and replace dead particles
        if( particles[i]->isDead())
        {
            delete particles[i];
            particles[i] = new Particle(position->x,position->y,imgLoader, renderer);
        }
    }

    //Show particles
    for( int i = 0; i < 10; ++i )
    {
        particles[ i ]->draw();
    }
}

void Ball::updateParticles(float velx, float vely){
	if(!particlesInitialized){
	for(int i=0;i<10;i++){
		particles[i] = new Particle(position->x,position->y,imgLoader, renderer);
		}
		particlesInitialized = true;
	}
	for( int i = 0; i < 10; ++i )
    {
        particles[ i ]->update(velx, vely);
    }
}

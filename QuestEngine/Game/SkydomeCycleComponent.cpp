#include "SkydomeCycleComponent.h"
#include "../Core/TimeManager.h"

SkydomeCycleComponent::SkydomeCycleComponent() : Component()
{

}

SkydomeCycleComponent::SkydomeCycleComponent(const SkydomeCycleComponent& skydomeCycleComponent) : Component(skydomeCycleComponent)
{
	skyDomeMaterial = skydomeCycleComponent.skyDomeMaterial;
	directionalLightComponent = skydomeCycleComponent.directionalLightComponent;
}

void SkydomeCycleComponent::Start()
{
	Component::Start();
}


void SkydomeCycleComponent::Update()
{
	Component::Update();


    time += TimeManager::Instance()->GetDeltaTime();

    if (time >= cycleDuration)
        time = 0.0f;

    float normalisedTime = time / cycleDuration;

    ColorRGB startColorA = ColorRGB();
    ColorRGB startColorB = ColorRGB();

    ColorRGB endColorA = ColorRGB();
    ColorRGB endColorB = ColorRGB();
    float interpolationTime = 0.0f;

    float sunAngle = 0.0f;

    if (normalisedTime >= sunRiseTime && normalisedTime < morningTime)
    {
        startColorA = sunRiseColorA;
        startColorB = sunRiseColorB;

        endColorA = morningColorA;
        endColorB = morningColorB;

        interpolationTime = (normalisedTime - sunRiseTime) / (morningTime - sunRiseTime);

        sunAngle = interpolationTime * 45.0f;
    }
    else if (normalisedTime >= morningTime && normalisedTime < afternoonTime)
    {
        startColorA = morningColorA;
        startColorB = morningColorB;

        endColorA = afternoonColorA;
        endColorB = afternoonColorB;

        interpolationTime = (normalisedTime - morningTime) / (afternoonTime - morningTime);

        sunAngle = 45.0f + interpolationTime * (130.f - 45.0f); 
    }
    else if (normalisedTime >= afternoonTime && normalisedTime < sunsetTime)
    {
        startColorA = afternoonColorA;
        startColorB = afternoonColorB;

        endColorA = sunsetColorA;
        endColorB = sunsetColorB;

        interpolationTime = (normalisedTime - afternoonTime) / (sunsetTime - afternoonTime);

        sunAngle = 130.f + interpolationTime * (180 - 130.f); 
    }
    else if (normalisedTime >= sunsetTime && normalisedTime < duskTime)
    {
        startColorA = sunsetColorA;
        startColorB = sunsetColorB;

        endColorA = duskColorA;
        endColorB = duskColorB;

        interpolationTime = (normalisedTime - sunsetTime) / (duskTime - sunsetTime);

        sunAngle = 180 + interpolationTime * (183 - 180); 
    }
    else if (normalisedTime >= duskTime || normalisedTime < nightTime)
    {
        startColorA = duskColorA;
        startColorB = duskColorB;

        endColorA = nightColorA;
        endColorB = nightColorB;

        if (normalisedTime >= duskTime)
            interpolationTime = (normalisedTime - duskTime) / ((1.0f - duskTime) + nightTime);
        else
            interpolationTime = ((1.0f - duskTime) + normalisedTime) / ((1.0f - duskTime) + nightTime);

        sunAngle = 183 + interpolationTime * (270 - 183);  
    }
    else if (normalisedTime >= nightTime && normalisedTime < dawnTime)
    {
        startColorA = nightColorA;
        startColorB = nightColorB;

        endColorA = dawnColorA;
        endColorB = dawnColorB;

        interpolationTime = (normalisedTime - nightTime) / (dawnTime - nightTime);

        sunAngle = 270 + interpolationTime * (357 - 270); 
    }
    else if (normalisedTime >= dawnTime && normalisedTime < sunRiseTime)
    {
        startColorA = dawnColorA;
        startColorB = dawnColorB;

        endColorA = sunRiseColorA;
        endColorB = sunRiseColorB;

        interpolationTime = (normalisedTime - dawnTime) / (sunRiseTime - dawnTime);

        sunAngle = 357 + interpolationTime * (360 - 357);  
    }

    ColorRGB colorA = ColorRGB::Lerp(startColorA, endColorA, interpolationTime);
    ColorRGB colorB = ColorRGB::Lerp(startColorB, endColorB, interpolationTime);
    skyDomeMaterial->SetColor("colorA", colorA);
    skyDomeMaterial->SetColor("colorB", colorB);

    Quaternion rotation = Quaternion::AxisAngle(Vector3D(0.0f, 1.0f, 0.0f), 90.0f);
	Quaternion sunRotation = Quaternion::AxisAngle(Vector3D(0.0f, 0.0f, 1.0f), -sunAngle);
    rotation.Rotate(sunRotation, true);
	directionalLightComponent->SetWorldRotation(rotation);


    Vector3D result = rotation * Vector3D(0, 0, 1);
    skyDomeMaterial->SetVector3D("lightDirection", directionalLightComponent->GetForwardVector());
}




SkydomeCycleComponent::~SkydomeCycleComponent()
{
}

Component* SkydomeCycleComponent::Clone()
{
	SkydomeCycleComponent* skydomeCycleComponent = new SkydomeCycleComponent(*this);
	clonnedObject = skydomeCycleComponent;
	clonnedObject->baseObject = this;
	return skydomeCycleComponent;
}

void SkydomeCycleComponent::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();

	SkydomeCycleComponent* baseSkyCycleComponent = (SkydomeCycleComponent*)baseObject;

	if (baseSkyCycleComponent->skyDomeMaterial != nullptr)
		skyDomeMaterial = baseSkyCycleComponent->skyDomeMaterial;

	if (baseSkyCycleComponent->directionalLightComponent != nullptr)
		directionalLightComponent = (DirectionalLightComponent*)baseSkyCycleComponent->directionalLightComponent->clonnedObject;

	
}

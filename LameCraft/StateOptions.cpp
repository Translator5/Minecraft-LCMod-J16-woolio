#include "StateOptions.h"
#include "InputHelper.h"
#include "TextureHelper.h"

StateOptions::StateOptions()
{

}

StateOptions::~StateOptions()
{

}

void StateOptions::Init()
{
	//set render manager instance
	mRender = RenderManager::InstancePtr();
	mSystemMgr = SystemManager::Instance();
	mSoundMgr = SoundManager::Instance();

	logoSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Logo),0,0,256,64);
	logoSprite->Scale(1.5f,1.5f);
	logoSprite->SetPosition(240,50);

	buttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Utils),24,22,200,20);
	buttonSprite->SetPosition(240,150);

	sbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Utils),24,62,200,20);
	sbuttonSprite->SetPosition(240,150);

	halfbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Utils),24,22,200,20);
	halfbuttonSprite->Scale(0.75f,1);
	halfbuttonSprite->SetPosition(240,150);

	halfsbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Utils),24,42,200,20);
	halfsbuttonSprite->Scale(0.75f,1);
	halfsbuttonSprite->SetPosition(240,150);

	backSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Dirt),0,0,16,16);
	backSprite->Scale(2,2);

	moverSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Utils),191,2,9,20);
	smoverSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Utils),182,2,9,20);

	lamecraftSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::lameCraft),0,0,256,64);
	lamecraftSprite->Scale(1.5f,1.5f);
	lamecraftSprite->SetPosition(240,50);

	selectPos = 0;
	menuState = 0;

	//controls
	controlPos = 0;
	controlStart = 0;
	controlEnd = 0;

	//key states
	chooseKeyState = false;
	configChanged = false;
	currentKey = -1;
	newKey = -1;

	currentAnalogPos = 0;
}

void StateOptions::Enter()
{
	mRender->SetOrtho(0,0,0,0,0,0);
	selectPos = 0;
}

void StateOptions::CleanUp()
{
	delete logoSprite;
	delete buttonSprite;
	delete sbuttonSprite;
	delete halfbuttonSprite;
	delete halfsbuttonSprite;
	delete backSprite;
	delete moverSprite;
	delete smoverSprite;
}

void StateOptions::Pause()
{

}

void StateOptions::Resume()
{

	menuState = 0;
	mRender->SetOrtho(0,0,0,0,0,0);
}

void StateOptions::HandleEvents(StateManager* sManager)
{
	//update input
	mSystemMgr->InputUpdate();

	switch(menuState)
	{
		case 0:
		{
			//up, down
			if(mSystemMgr->KeyPressed(PSP_CTRL_UP))
			{
				selectPos--;
				if(selectPos < 0)
					selectPos = 2;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
			{
				selectPos++;
				if(selectPos > 2)
					selectPos = 0;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
			{
				InputHelper::Instance()->Save();
				sManager->PopState();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
			{
				if(selectPos == 0)
				{
					menuState = 1;

					controlPos = 0;
					controlStart = 0;
					controlEnd = 6;

					//reset key state settings
					chooseKeyState = false;
					configChanged = false;
					currentKey = -1;
					newKey = -1;

				}
				if(selectPos == 1)
				{
					menuState = 2;
					currentAnalogPos = 0;
				}
				if(selectPos == 2)
				{
					InputHelper::Instance()->Save();
					sManager->PopState();
				}
			}
		}
		break;
		case 1:
		{
			if(chooseKeyState)//here we set new key for action
			{
				//check each button
				if(mSystemMgr->KeyPressed(PSP_CTRL_UP))
					currentKey = 0;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
					currentKey = 1;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_LEFT))
					currentKey = 2;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_RIGHT))
					currentKey = 3;//cross

				if(mSystemMgr->KeyPressed(PSP_CTRL_TRIANGLE))
					currentKey = 4;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
					currentKey = 5;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_SQUARE))
					currentKey = 6;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
					currentKey = 7;//cross

				if(mSystemMgr->KeyPressed(PSP_CTRL_LTRIGGER))
					currentKey = 8;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_RTRIGGER))
					currentKey = 9;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_SELECT))
					currentKey = 10;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_START))
					currentKey = 15;//cross

				if(mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)//analog up
					currentKey = 12;//cross
				if(mSystemMgr->GetAnalogY() < InputHelper::Instance()->analogYdown)//analog down
					currentKey = 11;//cross
				if(mSystemMgr->GetAnalogX() < InputHelper::Instance()->analogXleft)//analog left
					currentKey = 13;//cross
				if(mSystemMgr->GetAnalogX() > InputHelper::Instance()->analogXright)//analog right
					currentKey = 14;//cross

				if(currentKey != -1)
				{
					//chek if it the same key
					if(InputHelper::Instance()->getButtonToAction(controlPos) != currentKey)
						configChanged = true;

					//set key
					InputHelper::Instance()->setButtonToAction(controlPos,currentKey);

					//quit
					chooseKeyState = false;

					mSoundMgr->PlayMenuSound();
				}

			}else
			{
				if(mSystemMgr->KeyPressed(PSP_CTRL_UP))
				{
					controlPos--;

					if(controlPos < controlStart)
					{
						controlStart--;
						controlEnd--;

						if(controlEnd < 6)
						{
							controlStart = 0;
							controlEnd = 6;
						}
					}

					if(controlPos < 0)
					{
						controlPos = 16;
						controlEnd = 16;
						controlStart = 10;
						if(controlStart < 0)
							controlStart = 0;
					}

					mSoundMgr->PlayMenuSound();
				}

				if(mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
				{
					controlPos++;

					if(controlPos == controlEnd)
					{
						controlStart++;
						controlEnd++;
						if(controlEnd > 16)//jest 15 opcji od 0
						{
							controlStart = 10;
							controlEnd = 16;
						}
					}

					if(controlPos > 16)
					{
						controlPos = 0;
						controlStart = 0;
						controlEnd = 6;
					}

					mSoundMgr->PlayMenuSound();
				}

				if(mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
				{
					menuState = 0;
				}

				if(mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
				{
					if(controlPos == 16)//back button - exit from this menu
					{
						//save if any key changed
						//if(configChanged)
						//	InputHelper::Instance()->Save();

						menuState = 0;
					}else//start key choosing
					{
						currentKey = -1;
						chooseKeyState = true;
					}
				}
			}
		}
		break;
		case 2:
		{
			if(mSystemMgr->KeyPressed(PSP_CTRL_UP))
			{
				currentAnalogPos--;

				if(currentAnalogPos < 0)
					currentAnalogPos = 4;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
			{
				currentAnalogPos++;
				if(currentAnalogPos > 4)
					currentAnalogPos = 0;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_LEFT))
			{
				//dodatnie
				if(currentAnalogPos == 0 || currentAnalogPos == 3)
				{
					if(currentAnalogPos == 0)
					{
						InputHelper::Instance()->analogYup -= 0.1f;
						if(InputHelper::Instance()->analogYup < 0.1f)
							InputHelper::Instance()->analogYup = 0.1f;
					}else
					{
						InputHelper::Instance()->analogXright -= 0.1f;
						if(InputHelper::Instance()->analogXright < 0.1f)
							InputHelper::Instance()->analogXright = 0.1f;
					}
				}
				//ujemne
				if(currentAnalogPos == 1 || currentAnalogPos == 2)
				{
					if(currentAnalogPos == 1)
					{
						InputHelper::Instance()->analogYdown += 0.1f;
						if(InputHelper::Instance()->analogYdown > -0.1f)
							InputHelper::Instance()->analogYdown = -0.1f;
					}else
					{
						InputHelper::Instance()->analogXleft += 0.1f;
						if(InputHelper::Instance()->analogXleft > -0.1f)
							InputHelper::Instance()->analogXleft = -0.1f;
					}
				}

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_RIGHT))
			{
				//dodatnie
				if(currentAnalogPos == 0 || currentAnalogPos == 3)
				{
					if(currentAnalogPos == 0)
					{
						InputHelper::Instance()->analogYup += 0.1f;
						if(InputHelper::Instance()->analogYup > 0.9f)
							InputHelper::Instance()->analogYup = 0.9f;
					}else
					{
						InputHelper::Instance()->analogXright += 0.1f;
						if(InputHelper::Instance()->analogXright > 0.9f)
							InputHelper::Instance()->analogXright = 0.9f;
					}
				}
				//ujemne
				if(currentAnalogPos == 1 || currentAnalogPos == 2)
				{
					if(currentAnalogPos == 1)
					{
						InputHelper::Instance()->analogYdown -= 0.1f;
						if(InputHelper::Instance()->analogYdown < -0.9f)
							InputHelper::Instance()->analogYdown = -0.9f;
					}else
					{
						InputHelper::Instance()->analogXleft -= 0.1f;
						if(InputHelper::Instance()->analogXleft < -0.9f)
							InputHelper::Instance()->analogXleft = -0.9f;
					}
				}

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
			{
				menuState = 0;
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
			{
				if(currentAnalogPos == 4)
					menuState = 0;
			}
		}
		break;
	}
}

void StateOptions::Update(StateManager* sManager)
{

}

void StateOptions::Draw(StateManager* sManager)
{
	//start rendering
	mRender->StartFrame();

	switch(menuState)
	{
		case 0:
		{
			sceGuDisable(GU_DEPTH_TEST);
			sceGuEnable(GU_BLEND);
			sceGuColor(GU_COLOR(1,1,1,1.0f));

			for(int x = 0;x < 16;x++)
			{
				for(int y = 0;y < 9;y++)
				{
					backSprite->SetPosition(x*32,y*32);
					backSprite->Draw();
				}
			}

			lamecraftSprite->Draw();

			//Controls
			buttonSprite->SetPosition(240,120);
			buttonSprite->Draw();

			//sensity - analog stick
			buttonSprite->SetPosition(240,160);
			buttonSprite->Draw();

			//back
			buttonSprite->SetPosition(240,200);
			buttonSprite->Draw();


			//selected button
			sbuttonSprite->SetPosition(240,(selectPos * 40) + 120);
			sbuttonSprite->Draw();

			sceGuDisable(GU_BLEND);
			sceGuEnable(GU_DEPTH_TEST);

			//draw subtitles on buttons

			mRender->DebugPrint(240,125,"Controls");
			mRender->DebugPrint(240,165,"Analog stick");
			mRender->DebugPrint(240,205,"Back");
		}
		break;
		case 1://controls
		{
			sceGuDisable(GU_DEPTH_TEST);
			sceGuEnable(GU_BLEND);
			sceGuColor(GU_COLOR(1,1,1,1.0f));

			for(int x = 0;x < 16;x++)
			{
				for(int y = 0;y < 9;y++)
				{
					backSprite->SetPosition(x*32,y*32);
					backSprite->Draw();
				}
			}

			int starty = 60;
			//something
			for(int i = controlStart;i < controlEnd;i++)
			{
				if(controlPos == i)
				{
					halfsbuttonSprite->SetPosition(160,starty + (i * 30) - (controlStart * 30));
					halfsbuttonSprite->Draw();
				}else
				{
					halfbuttonSprite->SetPosition(160,starty + (i * 30) - (controlStart * 30));
					halfbuttonSprite->Draw();
				}
			}

			//back
			if(controlPos == 16)
			{
				sbuttonSprite->SetPosition(240,260);
				sbuttonSprite->Draw();
			}else
			{
				buttonSprite->SetPosition(240,260);
				buttonSprite->Draw();
			}

			sceGuDisable(GU_BLEND);
			sceGuEnable(GU_DEPTH_TEST);

			//write action names
			starty = 67;
			mRender->SetFontStyle(0.7f,0xFFFFFFFF,0xFF000000,0x00000000);
			for(int i = controlStart;i < controlEnd;i++)
			{
				//action
				mRender->DebugPrint(250,starty + (i * 30) - (controlStart * 30),InputHelper::Instance()->getActionName(i).c_str());
			}

			mRender->SetFontStyle(0.6f,0xFFFFFFFF,0xFF000000,0x00000200);
			starty = 65;
			for(int i = controlStart;i < controlEnd;i++)
			{
				//button assigned to this action
				if(controlPos == i && chooseKeyState == true)
					mRender->DebugPrint(160,starty + (i * 30) - (controlStart * 30),"...");
				else
					mRender->DebugPrint(160,starty + (i * 30) - (controlStart * 30),InputHelper::Instance()->getButtonName(InputHelper::Instance()->getConnection(i).button).c_str());
			}

			mRender->SetFontStyle(0.5f,0xFFFFFFFF,0xFF000000,0x00000200);
			mRender->DebugPrint(240,265,"Back");
			mRender->DebugPrint(240,20,"CONTROLS");
		}
		break;
		case 2://analog stick
		{
			sceGuDisable(GU_DEPTH_TEST);
			sceGuEnable(GU_BLEND);
			sceGuColor(GU_COLOR(1,1,1,1.0f));

			for(int x = 0;x < 16;x++)
			{
				for(int y = 0;y < 9;y++)
				{
					backSprite->SetPosition(x*32,y*32);
					backSprite->Draw();
				}
			}

			lamecraftSprite->Draw();

			//something
			buttonSprite->SetPosition(240,100);
			buttonSprite->Draw();

			buttonSprite->SetPosition(240,130);
			buttonSprite->Draw();

			buttonSprite->SetPosition(240,160);
			buttonSprite->Draw();

			buttonSprite->SetPosition(240,190);
			buttonSprite->Draw();

			//back
			buttonSprite->SetPosition(240,260);
			buttonSprite->Draw();

			if(currentAnalogPos < 4)
			{
				sbuttonSprite->SetPosition(240,100 + (currentAnalogPos * 30));
				sbuttonSprite->Draw();
			}else
			{
				sbuttonSprite->SetPosition(240,260);
				sbuttonSprite->Draw();
			}

			moverSprite->SetPosition(144 +((fabs(InputHelper::Instance()->analogYup) * 10) * 20),100);
			moverSprite->Draw();

			moverSprite->SetPosition(144 +((fabs(InputHelper::Instance()->analogYdown) * 10) * 20),130);
			moverSprite->Draw();

			moverSprite->SetPosition(144 +((fabs(InputHelper::Instance()->analogXleft) * 10) * 20),160);
			moverSprite->Draw();

			moverSprite->SetPosition(144 +((fabs(InputHelper::Instance()->analogXright) * 10) * 20),190);
			moverSprite->Draw();

			sceGuDisable(GU_BLEND);
			sceGuEnable(GU_DEPTH_TEST);

			mRender->DebugPrint(240,105,"Analog up : %d%%",(int)(fabs(InputHelper::Instance()->analogYup) * 100.0f));
			mRender->DebugPrint(240,135,"Analog down : %d%%",(int)(fabs(InputHelper::Instance()->analogYdown) * 100.0f));
			mRender->DebugPrint(240,165,"Analog left : %d%%",(int)(fabs(InputHelper::Instance()->analogXleft) * 100.0f));
			mRender->DebugPrint(240,195,"Analog right : %d%%",(int)(fabs(InputHelper::Instance()->analogXright) * 100.0f));

			mRender->DebugPrint(240,265,"Back");
		}
		break;
	}

	//end frame
	mRender->EndFrame();
}


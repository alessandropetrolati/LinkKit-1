// Copyright: 2018, Ableton AG, Berlin. All rights reserved.

#include <UIKit/UIKit.h>
#include "ABLLink.h"
#include "QuantumView.h"

@interface TransportButton : UIButton
@end

@interface ViewController : UIViewController

@property (weak, nonatomic) IBOutlet UIButton *transportButton;
@property (weak, nonatomic) IBOutlet UILabel *bpmLabel;
@property (weak, nonatomic) IBOutlet UILabel *quantumLabel;
@property (weak, nonatomic) IBOutlet UILabel *beatTimeLabel;
@property (weak, nonatomic) IBOutlet QuantumView *quantumView;
@property (readonly, nonatomic) ABLLinkRef linkRef;

- (IBAction)transportButtonAction:(TransportButton *)sender;
- (IBAction)bpmIncreaseTouchDownAction:(UIButton *)sender;
- (IBAction)bpmIncreaseTouchUpInsideAction:(UIButton *)sender;
- (IBAction)bpmIncreaseTouchUpOutsideAction:(UIButton *)sender;
- (IBAction)bpmDecreaseTouchDownAction:(UIButton *)sender;
- (IBAction)bpmDecreaseTouchUpInsideAction:(UIButton *)sender;
- (IBAction)bpmDecreaseTouchUpOutsideAction:(UIButton *)sender;
- (IBAction)quantumIncreaseAction:(UIButton *)sender;
- (IBAction)quantumDecreaseAction:(UIButton *)sender;
- (IBAction)showLinkSettings:(UIButton *)sender;

- (void)enableAudioEngine:(BOOL)enable;
@property (nonatomic, getter=isPlaying, readonly) BOOL playing;

@end

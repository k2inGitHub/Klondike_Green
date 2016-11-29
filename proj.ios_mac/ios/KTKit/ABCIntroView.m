//
//  IntroView.m
//  DrawPad
//
//  Created by Adam Cooper on 2/4/15.
//  Copyright (c) 2015 Adam Cooper. All rights reserved.
//

#import "ABCIntroView.h"
#if ENABLE_HL
//#import "HLService.h"
#endif

@interface ABCIntroView () <UIScrollViewDelegate>
@property (strong, nonatomic) UIScrollView *scrollView;
@property (strong, nonatomic) UIPageControl *pageControl;
@property (strong, nonatomic) UIButton *doneButton;

@property (strong, nonatomic) UIView *viewOne;
@property (strong, nonatomic) UIView *viewTwo;
@property (strong, nonatomic) UIView *viewThree;
@property (strong, nonatomic) UIView *viewFour;
@property (strong, nonatomic) UIView *viewFive;

@property (nonatomic, strong) NSArray *bgColors;

@end

@implementation ABCIntroView

- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if(self){
        
        _bgColors = @[[UIColor colorWithRed:250.f/255.f green:197.f/255.f blue:31.f/255.f alpha:1],
                      [UIColor colorWithRed:88.f/255.f green:183.f/255.f blue:247.f/255.f alpha:1],
                      [UIColor colorWithRed:148.f/255.f green:153.f/255.f blue:253.f/255.f alpha:1],
                      [UIColor colorWithRed:56.f/255.f green:220.f/255.f blue:183.f/255.f alpha:1],
                      [UIColor whiteColor]];
        
        [self addSubview:self.scrollView];
        [self addSubview:self.pageControl];
    
        [self.scrollView addSubview:self.viewOne];
        [self.scrollView addSubview:self.viewTwo];
        [self.scrollView addSubview:self.viewThree];
        [self.scrollView addSubview:self.viewFour];
        [self.scrollView addSubview:self.viewFive];
        
        //Done Button
        [self.viewFive addSubview:self.doneButton];
            
        
    }
    return self;
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView {
    
    CGFloat pageWidth = CGRectGetWidth(self.bounds);
    CGFloat pageFraction = self.scrollView.contentOffset.x / pageWidth;
    NSInteger lastpage = self.pageControl.currentPage;
    self.pageControl.currentPage = roundf(pageFraction);
    if (lastpage != self.pageControl.currentPage) {
#if ENABLE_HL
        [[HLAdManager sharedInstance] showUnsafeInterstitial];
#endif
    }
}

-(UIView *)viewOne {
    if (!_viewOne) {
    
        _viewOne = [[UIView alloc] initWithFrame:self.frame];
        _viewOne.backgroundColor = _bgColors[0];
        
        UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, self.frame.size.height*.05, self.frame.size.width*.8, 60)];
        titleLabel.center = CGPointMake(self.center.x, self.frame.size.height*.125);
        titleLabel.text = [NSString stringWithFormat:@"一元起拍"];
        titleLabel.font = [UIFont systemFontOfSize:40.0];
        titleLabel.textColor = [UIColor whiteColor];
        titleLabel.textAlignment =  NSTextAlignmentCenter;
        titleLabel.numberOfLines = 0;
        [_viewOne addSubview:titleLabel];
        
        UIImageView *imageview = [[UIImageView alloc] initWithFrame:CGRectMake(self.frame.size.width*.1, self.frame.size.height*.1, self.frame.size.width*.8, self.frame.size.width)];
        imageview.center = CGPointMake(self.center.x, self.frame.size.height*.54);
        imageview.contentMode = UIViewContentModeScaleAspectFit;
        imageview.image = [UIImage imageNamed:@"Intro_Screen_One"];
        [_viewOne addSubview:imageview];
        
        UILabel *descriptionLabel = [[UILabel alloc] initWithFrame:CGRectMake(self.frame.size.width*.1, self.frame.size.height*.2, self.frame.size.width*.8, 60)];
        descriptionLabel.text = [NSString stringWithFormat:@"真 · 会过日子"];
        descriptionLabel.font = [UIFont systemFontOfSize:18.0];
        descriptionLabel.textColor = [UIColor whiteColor];
        descriptionLabel.textAlignment =  NSTextAlignmentCenter;
        descriptionLabel.numberOfLines = 0;
        [descriptionLabel sizeToFit];
        [_viewOne addSubview:descriptionLabel];
        
        CGPoint labelCenter = CGPointMake(self.center.x, self.frame.size.height*.2);
        descriptionLabel.center = labelCenter;
        
    }
    return _viewOne;
    
}

-(UIView *)viewTwo {
    if (!_viewTwo) {
        CGFloat originWidth = self.frame.size.width;
        CGFloat originHeight = self.frame.size.height;
        
        _viewTwo = [[UIView alloc] initWithFrame:CGRectMake(originWidth, 0, originWidth, originHeight)];
        _viewTwo.backgroundColor = _bgColors[1];
        
        UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, self.frame.size.height*.05, self.frame.size.width*.8, 60)];
        titleLabel.center = CGPointMake(self.center.x, self.frame.size.height*.125);
        titleLabel.text = [NSString stringWithFormat:@"正品超值"];
        titleLabel.font = [UIFont systemFontOfSize:40.0];
        titleLabel.textColor = [UIColor whiteColor];
        titleLabel.textAlignment =  NSTextAlignmentCenter;
        titleLabel.numberOfLines = 0;
        [_viewTwo addSubview:titleLabel];
        
        UIImageView *imageview = [[UIImageView alloc] initWithFrame:CGRectMake(self.frame.size.width*.1, self.frame.size.height*.1, self.frame.size.width*.8, self.frame.size.width)];
        imageview.contentMode = UIViewContentModeScaleAspectFit;
        imageview.image = [UIImage imageNamed:@"Intro_Screen_Two"];
        imageview.center = CGPointMake(self.center.x, self.frame.size.height*.54);
        [_viewTwo addSubview:imageview];
        
        UILabel *descriptionLabel = [[UILabel alloc] initWithFrame:CGRectMake(self.frame.size.width*.1, self.frame.size.height*.7, self.frame.size.width*.8, 60)];
        descriptionLabel.text = [NSString stringWithFormat:@"真 · 会过日子"];
        descriptionLabel.font = [UIFont systemFontOfSize:18.0];
        descriptionLabel.textColor = [UIColor whiteColor];
        descriptionLabel.textAlignment =  NSTextAlignmentCenter;
        descriptionLabel.numberOfLines = 0;
        [descriptionLabel sizeToFit];
        [_viewTwo addSubview:descriptionLabel];
        
        CGPoint labelCenter = CGPointMake(self.center.x, self.frame.size.height*.2);
        descriptionLabel.center = labelCenter;
    }
    return _viewTwo;
    
}

-(UIView *)viewThree{
    
    if (!_viewThree) {
        CGFloat originWidth = self.frame.size.width;
        CGFloat originHeight = self.frame.size.height;
        
        _viewThree = [[UIView alloc] initWithFrame:CGRectMake(originWidth*2, 0, originWidth, originHeight)];
        _viewThree.backgroundColor = _bgColors[2];
        
        UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, self.frame.size.height*.05, self.frame.size.width*.8, 60)];
        titleLabel.center = CGPointMake(self.center.x, self.frame.size.height*.125);
        titleLabel.text = [NSString stringWithFormat:@"火速发货"];
        titleLabel.font = [UIFont systemFontOfSize:40.0];
        titleLabel.textColor = [UIColor whiteColor];
        titleLabel.textAlignment =  NSTextAlignmentCenter;
        titleLabel.numberOfLines = 0;
        [_viewThree addSubview:titleLabel];
        
        UIImageView *imageview = [[UIImageView alloc] initWithFrame:CGRectMake(self.frame.size.width*.1, self.frame.size.height*.1, self.frame.size.width*.8, self.frame.size.width)];
        imageview.contentMode = UIViewContentModeScaleAspectFit;
        imageview.image = [UIImage imageNamed:@"Intro_Screen_Three"];
        imageview.center = CGPointMake(self.center.x, self.frame.size.height*.54);
        [_viewThree addSubview:imageview];
        
        
        UILabel *descriptionLabel = [[UILabel alloc] initWithFrame:CGRectMake(self.frame.size.width*.1, self.frame.size.height*.7, self.frame.size.width*.8, 60)];
        descriptionLabel.text = [NSString stringWithFormat:@"真 · 会过日子"];
        descriptionLabel.font = [UIFont systemFontOfSize:18.0];
        descriptionLabel.textColor = [UIColor whiteColor];
        descriptionLabel.textAlignment =  NSTextAlignmentCenter;
        descriptionLabel.numberOfLines = 0;
        [descriptionLabel sizeToFit];
        [_viewThree addSubview:descriptionLabel];
        
        CGPoint labelCenter = CGPointMake(self.center.x, self.frame.size.height*.2);
        descriptionLabel.center = labelCenter;
    }
    return _viewThree;
    
}

-(UIView *)viewFour {
    if (!_viewFour) {
    
        CGFloat originWidth = self.frame.size.width;
        CGFloat originHeight = self.frame.size.height;
        
        _viewFour = [[UIView alloc] initWithFrame:CGRectMake(originWidth*3, 0, originWidth, originHeight)];
        _viewFour.backgroundColor = _bgColors[3];
        
        UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, self.frame.size.height*.05, self.frame.size.width*.8, 60)];
        titleLabel.center = CGPointMake(self.center.x, self.frame.size.height*.125);
        titleLabel.text = [NSString stringWithFormat:@"支付保障"];
        titleLabel.font = [UIFont systemFontOfSize:40.0];
        titleLabel.textColor = [UIColor whiteColor];
        titleLabel.textAlignment =  NSTextAlignmentCenter;
        titleLabel.numberOfLines = 0;
        [_viewFour addSubview:titleLabel];
        
        UIImageView *imageview = [[UIImageView alloc] initWithFrame:CGRectMake(self.frame.size.width*.1, self.frame.size.height*.1, self.frame.size.width*.8, self.frame.size.width)];
        imageview.contentMode = UIViewContentModeScaleAspectFit;
        imageview.image = [UIImage imageNamed:@"Intro_Screen_Four"];
        imageview.center = CGPointMake(self.center.x, self.frame.size.height*.54);
        [_viewFour addSubview:imageview];
        
        UILabel *descriptionLabel = [[UILabel alloc] initWithFrame:CGRectMake(self.frame.size.width*.1, self.frame.size.height*.7, self.frame.size.width*.8, 60)];
        descriptionLabel.text = [NSString stringWithFormat:@"真 · 会过日子"];
        descriptionLabel.font = [UIFont systemFontOfSize:18.0];
        descriptionLabel.textColor = [UIColor whiteColor];
        descriptionLabel.textAlignment =  NSTextAlignmentCenter;
        descriptionLabel.numberOfLines = 0;
        [descriptionLabel sizeToFit];
        [_viewFour addSubview:descriptionLabel];
        
        CGPoint labelCenter = CGPointMake(self.center.x, self.frame.size.height*.2);
        descriptionLabel.center = labelCenter;
        
    }
    return _viewFour;
    
}


-(UIView *)viewFive {
    if (!_viewFive) {
        
        CGFloat originWidth = self.frame.size.width;
        CGFloat originHeight = self.frame.size.height;
        
        _viewFive = [[UIView alloc] initWithFrame:CGRectMake(originWidth*4, 0, originWidth, originHeight)];
        _viewFive.backgroundColor = _bgColors[4];
        
        UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, self.frame.size.height*.05, self.frame.size.width*.8, 60)];
        titleLabel.center = CGPointMake(self.center.x, self.frame.size.height*.333);
        titleLabel.text = [NSString stringWithFormat:@"夺宝岛"];
        titleLabel.font = [UIFont boldSystemFontOfSize:50];
        titleLabel.textColor = [UIColor redColor];
        titleLabel.textAlignment =  NSTextAlignmentCenter;
        titleLabel.numberOfLines = 0;
        [_viewFive addSubview:titleLabel];
        
//        UIImageView *imageview = [[UIImageView alloc] initWithFrame:CGRectMake(self.frame.size.width*.1, self.frame.size.height*.1, self.frame.size.width*.8, self.frame.size.width)];
//        imageview.contentMode = UIViewContentModeScaleAspectFit;
//        imageview.image = [UIImage imageNamed:@"Intro_Screen_Four"];
//        imageview.center = CGPointMake(self.center.x, self.frame.size.height*.54);
//        [_viewFour addSubview:imageview];
        
        UILabel *descriptionLabel = [[UILabel alloc] initWithFrame:CGRectMake(self.frame.size.width*.1, self.frame.size.height*.7, self.frame.size.width*.8, 60)];
        descriptionLabel.text = [NSString stringWithFormat:@"真 · 会过日子"];
        descriptionLabel.font = [UIFont systemFontOfSize:18.0];
        descriptionLabel.textColor = [UIColor redColor];
        descriptionLabel.textAlignment =  NSTextAlignmentCenter;
        descriptionLabel.numberOfLines = 0;
        [descriptionLabel sizeToFit];
        [_viewFive addSubview:descriptionLabel];
        
        CGPoint labelCenter = CGPointMake(self.center.x, self.frame.size.height*.416);
        descriptionLabel.center = labelCenter;
        
    }
    return _viewFive;
    
}

-(UIScrollView *)scrollView {
    if (!_scrollView) {
        _scrollView = [[UIScrollView alloc] initWithFrame:self.frame];
        [_scrollView setDelegate:self];
        [_scrollView setPagingEnabled:YES];
        [_scrollView setContentSize:CGSizeMake(self.frame.size.width*5, 0)];
        [self.scrollView setContentOffset:CGPointMake(0, 0) animated:YES];
        _scrollView.showsVerticalScrollIndicator = _scrollView.showsHorizontalScrollIndicator = NO;
        _scrollView.alwaysBounceVertical = NO;
        _scrollView.alwaysBounceHorizontal = YES;
        
    }
    return _scrollView;
}

-(UIPageControl *)pageControl {
    if (!_pageControl) {
        _pageControl = [[UIPageControl alloc] initWithFrame:CGRectMake(0, self.frame.size.height-40, self.frame.size.width, 10)];
        [_pageControl setCurrentPageIndicatorTintColor:[UIColor grayColor]];
        [_pageControl setNumberOfPages:5];
    }
    return _pageControl;
}

-(UIButton *)doneButton {
    if (!_doneButton) {
        _doneButton = [[UIButton alloc] initWithFrame:CGRectMake(0, self.frame.size.height-100, 215, 50)];
        _doneButton.center = CGPointMake(self.center.x, self.frame.size.height-100);

        [_doneButton setBackgroundImage:[UIImage imageNamed:@"btn_intro"] forState:UIControlStateNormal];
//        [_doneButton setTintColor:[UIColor redColor]];
        [_doneButton setTitle:@"立即进入" forState:UIControlStateNormal];
        [_doneButton setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
        [_doneButton.titleLabel setFont:[UIFont systemFontOfSize:18.0]];
//        [_doneButton setBackgroundColor:[UIColor colorWithRed:0.129 green:0.588 blue:0.953 alpha:1.000]];
        [_doneButton addTarget:self.delegate action:@selector(onDoneButtonPressed) forControlEvents:UIControlEventTouchUpInside];
    }
    return _doneButton;
}

@end
       
     /**
     * jQuery Smooth Tabs
     * 
     * Author: Alexander Petkov
     * E-mail: petkov@webdesign-bulgaria.com
     * URL: http://www.webdesign-bulgaria.com
     * Copyright: (c) 2010 - Alexander Petkov, Webdesign Bulgaria Group Ltd.
     *
     * Example:
     * $('.smoothTabs').smoothTabs(FADE-SPEED-IN-MILISECONDS);
     *  
     * Works for:
     * <div class="smoothTabs">
     *     <ul>
     *         <li>tab 1</li>
     *         <li>tab 2</li>
     *     </ul>
     *     <div>
     *         content for tab 1
     *     </div>
     *     <div>
     *         content for tab 2
     *     </div>
     * </div>
     *
     * Tested with jQuery 1.3.2 
     *
     * LICENSE:
     *
     * Copyright (c) 2010 Alexander Petkov, http://www.webdesign-bulgaria.com
     *
     * Permission is hereby granted, free of charge, to any person obtaining
     * a copy of this software and associated documentation files (the
     * "Software"), to deal in the Software without restriction, including
     * without limitation the rights to use, copy, modify, merge, publish,
     * distribute, sublicense, and/or sell copies of the Software, and to
     * permit persons to whom the Software is furnished to do so, subject to
     * the following conditions:
     *
     * The above copyright notice and this permission notice shall be
     * included in all copies or substantial portions of the Software.
     * 
     * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
     * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
     * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
     * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
     * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
     * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
     * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
     */
     
     	(function($) {  
            $.fn.smoothTabs = function(fadeSpeed) {
                // Clicked tab class
                var $smoothTabsLiCurrent = 'current';
                // Hidden div class
                var $smoothTabsDivHidden = 'hidden';
                // Visible div class
                var $smoothTabsDivVisible = 'visible';
                
                // Makes first tab current, hides all divs and fades in the first one
                this.each(function() {
                    $("ul li:first", this).addClass($smoothTabsLiCurrent);
                    $(this).children("div.tab").addClass($smoothTabsDivHidden);
                    $("div:first", this).fadeIn(fadeSpeed)
                                        .addClass($smoothTabsDivVisible)
                                        .removeClass($smoothTabsDivHidden);
    			});

                // Tab click function
                $('ul li', this).click(function(){
                    var $parentUl = $(this).parent();
                    var $parentDiv = $($parentUl).parent();
                    $('li', $parentUl).removeClass($smoothTabsLiCurrent);
                    $(this).addClass($smoothTabsLiCurrent);
                    var $clickedIndex = $('li', $parentUl).index(this);
                    var $currentDiv = $('div.tab', $parentDiv).get($clickedIndex);
                    
                    // If current tab is clicked - we're done
                    if ($($currentDiv).attr('class') == $smoothTabsDivVisible) {
                        return false;
                    }
                    
                    // Current div is replaced by the selected one
                    $('.'+$smoothTabsDivVisible, $parentDiv).fadeOut(fadeSpeed, function(){
                        $($currentDiv).fadeIn(fadeSpeed).addClass($smoothTabsDivVisible).removeClass($smoothTabsDivHidden);
                    });
                    $('.'+$smoothTabsDivVisible, $parentDiv).removeClass($smoothTabsDivVisible).addClass($smoothTabsDivHidden);
                });
            };
    	})(jQuery); 
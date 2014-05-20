  var state = 0; // -1 is projects, 0 is normal, 1 is people
  var bIsSearch = false;

  var counter = 0;
  var bIsSwiping = false;

  var dataForSearch = [];
  var allData;

  //Initialize socket and get data
  var socket = io.connect('http://' + window.location.hostname + ':8080');
  socket.on('connect', function() {
    console.log('connected!')
  });

  socket.on('init', function (data) {
    createContent(data);
    allData = data;
  });

  //Create content from data on page

  function createContent(data) {
    data = sortByKey(data, 'project');
    for (var i = 0; i < data.length; i++) {
      var p = document.createElement('div');
      p.classList.add('project');
      p.innerHTML = '<h3>' + data[i].project + '</h3><h4>' + data[i].people + '</h4>';
      p.setAttribute('data-project', data[i].project);
      p.setAttribute('data-people', data[i].people);
      p.setAttribute('data-slug', data[i].slug);
      $('#project-list').append( p );
    }

    data = sortByKey(data, 'people');
    for (var i = 0; i < data.length; i++) {
      dataForSearch.push( data[i].people);

      var n = document.createElement('div');
      n.classList.add('person');
      n.innerHTML = '<h3>' + data[i].people + '</h3><h4>' + data[i].project + '</h4>';
      n.setAttribute('data-project', data[i].project);
      n.setAttribute('data-people', data[i].people);
      n.setAttribute('data-slug', data[i].slug);
      $('#people-list').append( n );

    }
  }

  //Send data to server if project/person is selected.

  $('body').on('tap', '.project', function(){
    var data = {
      project: $(this).attr('data-project'), 
      people: $(this).attr('data-people'),
      slug: $(this).attr('data-slug')
    };
    
    console.log('touch');

    socket.emit('load_model', data);
  });

  $('body').on('tap', '.person', function(){
    var data = {
      project: $(this).attr('data-project'), 
      people: $(this).attr('data-people'),
      slug: $(this).attr('data-slug')
    };

    console.log('touch');

    socket.emit('load_model', data);
  });

  //Search
  $('#search-input').autocomplete({
    minLength: 2,
    source: function(request, response) {
      var results = $.ui.autocomplete.filter(dataForSearch, request.term);
      response(results.slice(0, 3));
    },
  });

  $('#search-input').on('autocompleteselect', function( event, ui ) {
    var result = ui.item.value;

    var people = document.querySelectorAll('.person');
    for ( var i = 0; i < people.length; i++ ) {
      if ( result == people[i].getAttribute('data-people') ) {
        var data = {
          project: people[i].getAttribute('data-project'), 
          people: people[i].getAttribute('data-people'),
          slug: people[i].getAttribute('data-slug')
        };

        console.log('search');

        socket.emit('load_model', data);
      }
    }

    search.style.height = 0;
    bIsSearch = false;
    $('#search-input').val('');
    $('#search-input').blur();
  });

  $('#search-input').blur(function() {
    bIsSearch = false;
    $('#search-input').val('');
  });

  /**************************************************************************************/
  /**********************************SWIPE/DRAG EVENTS***********************************/
  /**************************************************************************************/

  var optionsX = {
    dragLockToAxis: true,
    dragBlockHorizontal: true,
  };
  var hammertimeX = new Hammer(document, optionsX);

  hammertimeX.on("swiperight", function(event) {
    event.gesture.preventDefault();

    if (!bIsSearch) {
      $('#search-input').blur();
      if (state == -1) {
        //do nothing
      }
      else if (state == 0) {
        $('#content').css('overflow', 'scroll');
        $('header').removeClass('header-visible').addClass('header-left');
        $('#project-list').removeClass('projects-hidden').addClass('projects-visible');
        state = -1;
      }
      else if (state == 1) {
        $('#content').scrollTop( 0 );
        $('header').removeClass('header-right').addClass('header-visible');
        $('#people-list').removeClass('people-visible').addClass('people-hidden');
        state = 0;
      }
      console.log('swiperight');
    }
  });

  hammertimeX.on("swipeleft", function(event) {
    event.gesture.preventDefault();

    if (!bIsSearch) {
      $('#search-input').blur();
      if (state == -1) {
        $('#content').scrollTop( 0 );
        $('header').removeClass('header-left').addClass('header-visible');
        $('#project-list').removeClass('projects-visible').addClass('projects-hidden');
        state = 0;
      }
      else if (state == 0) {
        $('header').removeClass('header-visible').addClass('header-right');
        $('#people-list').removeClass('people-hidden').addClass('people-visible');
        state = 1;
      }
      else if (state == 1) {
        //do nothing
      }
      console.log('swipeleft');
    }
  });

  //Swipe left for projects
  var swipeLeft = document.querySelector('#swipe-left .arrow');
  var swipeLeftInit = swipeLeft.style.width;
  var headerLeft = new Hammer( swipeLeft, optionsX );

  headerLeft.on("dragright", function(event) {
    bIsSwiping = true;

    var pos = getPosition(swipeLeft);
    if ( event.gesture.center.pageX < $(window).width() - 50) {
      swipeLeft.style.width = event.gesture.center.pageX - pos.x;
    }
  });

  headerLeft.on("release", function(event) {
    bIsSwiping = false;

    if ( event.gesture.center.pageX > $(window).width() - 50 ) {
        $('#content').css('overflow', 'scroll');
        $('header').removeClass('header-visible').addClass('header-left');
        $('#project-list').removeClass('projects-hidden').addClass('projects-visible');
        state = -1;    
        
        swipeLeft.style.width = swipeLeftInit;
      }
    else {
      swipeLeft.style.width = swipeLeftInit;
    }
  });

  //Swipe right for people
  var swipeRight = document.querySelector('#swipe-right .arrow');
  var swipeRightInit = swipeRight.style.width;
  var headerRight = new Hammer( swipeRight, optionsX );

  headerRight.on("dragleft", function(event) {
    bIsSwiping = true;

    var pos = getPosition(swipeRight);
    var right = pos.x + swipeRight.offsetWidth;

    if ( event.gesture.center.pageX > 75) {
      swipeRight.style.width = right - event.gesture.center.pageX;
    }
  });

  headerRight.on("release", function(event) {
    bIsSwiping = false;

    if ( event.gesture.center.pageX < 75 ) {
        $('header').removeClass('header-visible').addClass('header-right');
        $('#people-list').removeClass('people-hidden').addClass('people-visible');
        state = 1;
        
        swipeRight.style.width = swipeRightInit;
      }
    else {
      swipeRight.style.width = swipeRightInit;
    }
  });


  //Swipe down for search.
  var optionsSearch = {
    preventDefault: true
  };

  var swipeDown = document.querySelector('#instruction-search');
  var search = document.querySelector('#search');
  var hammerGetSearch = new Hammer( swipeDown, optionsSearch );

  hammerGetSearch.on("dragdown", function(event) {
    $('#search-input').val('');
    var pos = getPosition(swipeDown);
    search.style.height = event.gesture.center.pageY - pos.y;
  });

  hammerGetSearch.on("release", function(event) {
    if ( event.gesture.center.pageY > document.body.offsetHeight / 2 ) {
      search.style.height = "100%";
      bIsSearch = true;
      $('#search-input').focus();
    }
    else {
      search.style.height = 0;
    }
  });  


  //Swipe up to get rid of search
  hammerLoseSearch = new Hammer( search, optionsSearch );

  hammerLoseSearch.on("dragup", function(event) {
    var pos = getPosition(swipeDown);
    search.style.height = event.gesture.center.pageY - pos.y;
  });

  hammerLoseSearch.on("release", function(event) {
    if ( event.gesture.center.pageY < document.body.offsetHeight / 2 ) {
      search.style.height = 0;
      bIsSearch = false;
      $('#search-input').val('');
      $('#search-input').blur();
    }
    else {
      search.style.height = "100%";
    }
  });  


  /**************************************************************************************/
  //Animate swipe

  function animate() {

    if (bIsSwiping) {
      $('#swipe-left').css( "opacity", "1.0" );
      $('#swipe-right').css( "opacity", "1.0" );
    }
    else {
      var leftFade = Math.sin( counter ) * 0.15 + 0.85;
      var rightFade = Math.cos( counter ) * 0.15 + 0.85;

      $('#swipe-left').css( "opacity", leftFade );
      $('#swipe-right').css( "opacity", rightFade );

      counter += 0.06;
    }


    window.requestAnimationFrame(animate);
  }

  animate();



  /**************************************************************************************/
  /************************************** HELPERS ***************************************/
  /**************************************************************************************/

  function sortByKey(array, key) {
    return array.sort(function(a, b) {
      var x = a[key];
      var y = b[key];

      if (typeof x == "string")
      {
          x = x.toLowerCase(); 
          y = y.toLowerCase();
      }

      return ((x < y) ? -1 : ((x > y) ? 1 : 0));
    });
  }

  function getPosition(element) {
    var xPosition = 0;
    var yPosition = 0;
  
    while(element) {
        xPosition += (element.offsetLeft - element.scrollLeft + element.clientLeft);
        yPosition += (element.offsetTop - element.scrollTop + element.clientTop);
        element = element.offsetParent;
    }
    return { x: xPosition, y: yPosition };
  }

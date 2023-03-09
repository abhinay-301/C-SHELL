import React from 'react';
import { useNavigate  } from 'react-router-dom';
import image from './homepageutils/images.jpeg';
const FrontPage = () => {
  const history = useNavigate();

  const handleSignIn = () => {
    history('/login');
  };

  const handleSignUp = () => {
    history('/signup');
  };

  return (
    <div className="front-page">
      <img src={image} alt="social media app" className="front-page__image" />
      <div className="front-page__buttons">
        <button className="front-page__signin" onClick={handleSignIn}>Sign In</button>
        <button className="front-page__signup" onClick={handleSignUp}>Sign Up</button>
      </div>
    </div>
  );
};

export default FrontPage;

// import React from 'react';
// import { useNavigate } from 'react-router-dom';
// import image from './homepageutils/images.jpeg';

// const FrontPage = () => {
// const history = useNavigate();

// const handleSignIn = () => {
// history('/login');
// };

// const handleSignUp = () => {
// history('/signup');
// };

// return (
// <div style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', justifyContent: 'center', height: '100vh' }}>
// <img src={image} alt="social media app" style={{ width: '50%', maxWidth: '500px', marginBottom: '2rem' }} />
// <div style={{ display: 'flex', flexDirection: 'row' }}>
// <button style={{ padding: '1rem', marginRight: '1rem', border: 'none', borderRadius: '5px', fontSize: '1.2rem', backgroundColor: '#2196f3', color: 'white', cursor: 'pointer' }} onClick={handleSignIn}>Sign In</button>
// <button style={{ padding: '1rem', marginRight: '1rem', border: 'none', borderRadius: '5px', fontSize: '1.2rem', backgroundColor: '#2196f3', color: 'white', cursor: 'pointer' }} onClick={handleSignUp}>Sign Up</button>
// </div>
// </div>
// );
// };

// export default FrontPage;
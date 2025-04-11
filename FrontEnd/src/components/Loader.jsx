import React from 'react';

const Loader = ({ error }) => {
    if (error) {
        return (
            <div className="error-container">
                <div className="error-icon">✖</div>
                <h2>{error.message}</h2>
                <ul>
                    {error.details.map((detail, index) => (
                        <li key={index}>{detail}</li>
                    ))}
                </ul>
            </div>
        );
    }

    return (
        <div className="loader-container">
            <div className="loader"></div>
            <p>Conectando al servidor...</p>
        </div>
    );
};

export default Loader;
import styled from 'styled-components'

import SuccessSvg from '../../../../assets/svg-icons/success-circle-icon.svg'

export const SuccessIcon = styled.div`
  width: 112px;
  height: 112px;
  background: url(${SuccessSvg});
  margin: 24px 0;
`

export const SuccessText = styled.span`
  font-family: 'Poppins';
  font-style: normal;
  font-weight: 600;
  font-size: 18px;
  line-height: 27px;

  text-align: center;
  letter-spacing: 0.02em;

  color: ${(p) => p.theme.color.text01};
`

export const SuccessDescription = styled.div`
  font-family: 'Poppins';
  font-style: normal;
  font-weight: 400;
  font-size: 12px;
  line-height: 20px;
  text-align: center;
  color: ${(p) => p.theme.color.text02};

  padding: 8px 16px;
  flex-grow: 1;
`

export const ButtonRow = styled.div`
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: row;
  width: 100%;
  padding-bottom: 32px;
`

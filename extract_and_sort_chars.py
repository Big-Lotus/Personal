#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
파일에서 문자를 추출하고 사전식으로 정렬하는 프로그램
"""

import sys
import argparse
from collections import Counter


def extract_and_sort_chars(file_path, unique_only=False, case_sensitive=True):
    """
    파일에서 문자를 추출하고 사전식으로 정렬합니다.
    
    Args:
        file_path (str): 읽을 파일 경로
        unique_only (bool): 중복 제거 여부
        case_sensitive (bool): 대소문자 구분 여부
    
    Returns:
        list: 정렬된 문자 리스트
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
    except FileNotFoundError:
        print(f"오류: 파일 '{file_path}'을 찾을 수 없습니다.")
        return []
    except Exception as e:
        print(f"오류: 파일을 읽는 중 문제가 발생했습니다: {e}")
        return []
    
    # 문자 추출
    chars = []
    for char in content:
        if char.isalpha():  # 알파벳 문자만 추출
            if not case_sensitive:
                char = char.lower()
            chars.append(char)
    
    # 중복 제거 및 정렬
    if unique_only:
        chars = sorted(set(chars))
    else:
        chars = sorted(chars)
    
    return chars


def extract_and_sort_with_count(file_path, case_sensitive=True):
    """
    파일에서 문자를 추출하고 빈도와 함께 사전식으로 정렬합니다.
    
    Args:
        file_path (str): 읽을 파일 경로
        case_sensitive (bool): 대소문자 구분 여부
    
    Returns:
        list: (문자, 빈도) 튜플의 정렬된 리스트
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
    except FileNotFoundError:
        print(f"오류: 파일 '{file_path}'을 찾을 수 없습니다.")
        return []
    except Exception as e:
        print(f"오류: 파일을 읽는 중 문제가 발생했습니다: {e}")
        return []
    
    # 문자 추출 및 빈도 계산
    chars = []
    for char in content:
        if char.isalpha():  # 알파벳 문자만 추출
            if not case_sensitive:
                char = char.lower()
            chars.append(char)
    
    # 빈도 계산
    char_count = Counter(chars)
    
    # 사전식으로 정렬
    sorted_chars = sorted(char_count.items())
    
    return sorted_chars


def main():
    parser = argparse.ArgumentParser(description='파일에서 문자를 추출하고 사전식으로 정렬합니다.')
    parser.add_argument('file_path', help='읽을 파일 경로')
    parser.add_argument('-u', '--unique', action='store_true', help='중복 제거')
    parser.add_argument('-i', '--ignore-case', action='store_true', help='대소문자 구분 안함')
    parser.add_argument('-c', '--count', action='store_true', help='문자 빈도와 함께 출력')
    
    args = parser.parse_args()
    
    if args.count:
        # 빈도와 함께 출력
        result = extract_and_sort_with_count(args.file_path, not args.ignore_case)
        if result:
            print("문자별 빈도 (사전식 정렬):")
            for char, count in result:
                print(f"'{char}': {count}")
    else:
        # 일반 정렬
        result = extract_and_sort_chars(args.file_path, args.unique, not args.ignore_case)
        if result:
            print("추출된 문자 (사전식 정렬):")
            if args.unique:
                print(' '.join(result))
            else:
                print(''.join(result))


if __name__ == "__main__":
    main()
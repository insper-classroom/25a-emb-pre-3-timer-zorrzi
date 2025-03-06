#!/usr/bin/env python3

import pytest


def pytest_addoption(parser):
    parser.addoption("--name", action="store")


@pytest.fixture(scope="session")
def name(request):
    name_value = request.config.option.name
    if name_value is None:
        pytest.skip()
    return name_value
